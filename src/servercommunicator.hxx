#ifndef SERVERCOMMUNICATOR_H
#define SERVERCOMMUNICATOR_H

#undef RAPIDJSON_HAS_STDSTRING
#define RAPIDJSON_HAS_STDSTRING 1

#include <string>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <libgen.h>
#include <stdio.h>

#include "apirequest.hxx"

class ServerCommunicator {
	public:
	ServerCommunicator(const std::string& server_url);
	bool authenticate(const std::string& pgpid);
	void getFullTree(void (*callback)(const std::string&, const std::string&,
									  void *), void *userdata);
	const std::string /* base64 */ getSecret(const std::string& path);
	int setSecret(const std::string& path, const std::string& content);
	int deleteSecret(const std::string& path);

	private:
	std::string server_url;
	HTTPSession session;
};

#endif
