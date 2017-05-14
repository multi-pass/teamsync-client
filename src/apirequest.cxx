#include "apirequest.hxx"


////////////////////////////////////////////////////////////////////////////////
//////    APIRequest
////////////////////////////////////////////////////////////////////////////////

APIRequest::APIRequest(HTTPMethod method, const std::string& server_url,
					   const std::string& api_route, HTTPSession& session)
	: session(session), method(method) {
	CURL *curl = this->session.curl;

	switch (this->method) {
	case GET:
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
		break;
	case POST:
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
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
}

void APIRequest::setPOSTFields(const std::string& postfields) {
	if (POST == this->method) {
		CURL *curl = this->session.curl;
		this->postfields = postfields;

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, this->postfields.c_str());
	}
}

APIResponse APIRequest::send() {
	APIResponse response;

	CURL *curl = this->session.curl;

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &(APIResponse::writeCallback));

	CURLcode op_success = curl_easy_perform(curl);

	if (CURLE_OK == op_success) {
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &(response.response_code));

		char *content_type = NULL;
		curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
		response.content_type = content_type;
	}

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

	// Default options
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
#if HAVE_CONFIG_H
	curl_easy_setopt(curl, CURLOPT_USERAGENT,
					 (PACKAGE_NAME "/" PACKAGE_VERSION));
#endif

	// Set cookie file to an invalid path
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");

	this->curl = curl;
}

HTTPSession::~HTTPSession() {
	if (this->curl) {
		curl_easy_cleanup(this->curl);
	}
}
