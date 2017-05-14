#include "apirequest.hxx"


////////////////////////////////////////////////////////////////////////////////
//////    APIRequest
////////////////////////////////////////////////////////////////////////////////

APIRequest::APIRequest(HTTPMethod method, const std::string& server_url,
					   const std::string& api_route, HTTPSession& session) {
	this->session = session;
	CURL *curl = session.curl;

	switch (method) {
	case GET:
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
		break;
	case POST:
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
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

APIResponse APIRequest::send() {
	APIResponse response;

	CURL *curl = this->session.curl;

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &(APIResponse::writeCallback));

	curl_easy_perform(curl);

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

	this->curl = curl;
}

HTTPSession::~HTTPSession() {
	if (this->curl) {
		curl_easy_cleanup(this->curl);
	}
}
