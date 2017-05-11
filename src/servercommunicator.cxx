#include "servercommunicator.hxx"

ServerCommunicator::ServerCommunicator(std::string server_url)
	: server_url(server_url) {

}

bool ServerCommunicator::authorize() {

}

void ServerCommunicator::getFullTree() {
int ServerCommunicator::addSecret() {

}

int ServerCommunicator::setSecret() {

}

int ServerCommunicator::deleteSecret() {

}



/// API classes

APIRequest::APIRequest(HTTPMethod method, std::string server_url,
						 std::string api_route) {
	if (!(this->curl = curl_easy_init())) {
		// TODO: Throw exception
	}

	// Default options
	curl_easy_setopt(this->curl, CURLOPT_NOPROGRESS, 1L);

	switch (method) {
	case GET:
		curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1L);
		break;
	case POST:
		curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1L);
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

	curl_easy_setopt(this->curl, CURLOPT_URL, request_uri.c_str());
}

APIResponse APIRequest::execute() {
	APIResponse response;

	size_t (*write_callback)(char *, size_t, size_t, void *);
	write_callback = &(APIResponse::writeCallback);

	curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, write_callback);

	curl_easy_perform(this->curl);

	return response;
}

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
