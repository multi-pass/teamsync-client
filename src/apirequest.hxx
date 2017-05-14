#ifndef APIREQUEST_H
#define APIREQUEST_H

#include <string>

#include <curl/curl.h>


enum HTTPMethod {
	GET,
	POST,
	PUT,
	DELETE
};

class APIResponse;

class HTTPSession {
	friend class APIRequest;

	public:
	HTTPSession();
	~HTTPSession();

	private:
	CURL *getCURLHandle();

	CURL *curl = NULL;
};

class APIRequest {
	public:
	APIRequest(HTTPMethod method, const std::string& server_url,
			   const std::string& api_route);

	APIResponse send();

};


class APIResponse {
	friend class APIRequest;

	public:
	std::string http_response;

	private:
	APIResponse();
	size_t receiveData(char *ptr, size_t size, size_t nmemb);

	// only for passing to cURL
	static size_t writeCallback(char *ptr, size_t size, size_t nmemb,
								void *userdata);
};

#endif
