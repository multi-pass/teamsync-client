#ifndef APIREQUEST_H
#define APIREQUEST_H

#include <string>

#include <curl/curl.h>

////////////////////////////////////////////////////////////////////////////////
/// Note:
///  These classes are all highly thread-unsafe!
///  Make sure that only one request is using a HTTPSession at the time.
////////////////////////////////////////////////////////////////////////////////


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
	CURL *curl = NULL;
};

class APIRequest {
	public:
	APIRequest(HTTPMethod method, const std::string& server_url,
			   const std::string& api_route, HTTPSession& session);

	APIResponse send();

	private:
	HTTPSession& session;
	HTTPMethod method;
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
