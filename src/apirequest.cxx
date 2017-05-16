#include "apirequest.hxx"


////////////////////////////////////////////////////////////////////////////////
//////    APIRequest
////////////////////////////////////////////////////////////////////////////////

APIRequest::APIRequest(HTTPMethod method, const std::string& server_url,
					   const std::string& api_route, HTTPSession& session)
	: session(session), method(method) {
	CURL *curl = this->session.curl;

	// Reset handle
	curl_easy_reset(curl);

	// Default options
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
#if HAVE_CONFIG_H
	curl_easy_setopt(curl, CURLOPT_USERAGENT,
					 (PACKAGE_NAME "/" PACKAGE_VERSION));
#endif

	switch (this->method) {
	case GET:
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
		break;
	case POST:
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		break;
	case PUT:
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		break;
	case DELETE:
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
		break;
	default:
		// Unsupported method
		// TODO: Throw exception
		break;
	}

	if (api_route[0] != '/') {
		// Invalid route (must begin with a slash)
		// TODO: Throw exception
	}

	std::string request_uri = (server_url + api_route);
	curl_easy_setopt(curl, CURLOPT_URL, request_uri.c_str());

	curl_slist *headers = this->headers;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Depth: infinity");
	headers = curl_slist_append(headers, "Connection: Keep-Alive");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "Expect:");
	this->headers = headers;
}

void APIRequest::setRequestBody(const std::string& request_body) {
	this->request_body = request_body;
	CURL *curl = this->session.curl;

	if (POST == this->method) {
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, request_body.length());
		curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, request_body.c_str());
	} else {
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, &(APIRequest::readHelper));
		// curl_easy_setopt(curl, CURLOPT_READDATA, &(this->request_body));
		curl_easy_setopt(curl, CURLOPT_READDATA, (void *)this);
		// curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, request_body.size());
	}
}

size_t APIRequest::readHelper(char *buffer, size_t size, size_t nitems,
								  void *request_obj) {
	APIRequest *req = (APIRequest *)request_obj;
	std::string& request_body = req->request_body;

	const size_t numc(std::min<size_t>(request_body.length(), (nitems * size)));

	strncpy(buffer, request_body.c_str(), numc);
	request_body.erase(0, numc); // remove the characters from string
	return numc;
}

APIResponse APIRequest::send() {
	APIResponse response;

	CURL *curl = this->session.curl;

	// Set headers
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, this->headers);

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &(APIResponse::writeCallback));

	CURLcode op_success = curl_easy_perform(curl);

	if (CURLE_OK == op_success) {
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &(response.response_code));

		char *content_type = NULL;
		curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
		response.content_type = content_type;
	}

#ifdef DEBUG
	fprintf(stderr, "\nHTTP %d\n%s\n\n", response.response_code, response.http_response.c_str());
#endif

	return response;
}


////////////////////////////////////////////////////////////////////////////////
//////    APIResponse
////////////////////////////////////////////////////////////////////////////////

APIResponse::APIResponse() {

}

size_t APIResponse::receiveData(char *ptr, size_t size, size_t nmemb) {
	size_t total_length = (nmemb * size);

	// Store response
	this->http_response = std::string(ptr, total_length);

	return total_length;
}

size_t APIResponse::writeCallback(char *ptr, size_t size, size_t nmemb,
								  void *response_obj) {
	return ((APIResponse *)response_obj)->receiveData(ptr, size, nmemb);
}


////////////////////////////////////////////////////////////////////////////////
//////    HTTPSession
////////////////////////////////////////////////////////////////////////////////

HTTPSession::HTTPSession() {
	CURL *curl;
	if (!(curl = curl_easy_init())) {
		// TODO: Throw exception
	}

	// Set cookie file to an invalid path
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");

	this->curl = curl;
}

HTTPSession::~HTTPSession() {
	if (this->curl) {
		curl_easy_cleanup(this->curl);
	}
}
