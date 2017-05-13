#ifndef SERVERCOMMUNICATOR_H
#define SERVERCOMMUNICATOR_H

#include <string>

#include <stdio.h>
#include <curl/curl.h>

class ServerCommunicator {
	public:
	ServerCommunicator(const std::string& server_url);
	bool authenticate(const std::string& pgpid);
	void getFullTree();
	int addSecret();
	int setSecret();
	int deleteSecret();

	private:
	std::string server_url;
};

enum HTTPMethod {
	GET,
	POST,
	PUT,
	DELETE
};

class APIResponse;

class APIRequest {
	public:
	APIRequest(HTTPMethod method, std::string server_url,
			   std::string api_route);
	APIResponse execute();

	private:
	CURL *curl;
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
