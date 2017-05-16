#ifndef APIREQUEST_H
#define APIREQUEST_H

#if HAVE_CONFIG_H
#include "../config.h"
#endif

#include <algorithm>
#include <string>

#include <string.h>

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

	// Note: This method does nothing when the method is not POST
	void setRequestBody(const std::string& request_body);

	APIResponse send();

	private:
	// only for passing to cURL
	static size_t readHelper(char *buffer, size_t size, size_t nitems,
								 void *request_obj);

	HTTPSession& session;
	HTTPMethod method;
	std::string request_body;
	curl_slist *headers = NULL;
};


class APIResponse {
	friend class APIRequest;

	public:
	long response_code;
	std::string content_type;
	std::string http_response;

	private:
	APIResponse();
	size_t receiveData(char *ptr, size_t size, size_t nmemb);

	// only for passing to cURL
	static size_t writeCallback(char *ptr, size_t size, size_t nmemb,
								void *userdata);
};

#endif
