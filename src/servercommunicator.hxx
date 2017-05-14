#ifndef SERVERCOMMUNICATOR_H
#define SERVERCOMMUNICATOR_H

#include <string>

#include <stdio.h>

#include "apirequest.hxx"

class ServerCommunicator {
	public:
	ServerCommunicator(const std::string& server_url);
	bool authenticate(const std::string& pgpid);
	void getFullTree(void (*callback)(const std::string&,
									  const std::string&));
	int addSecret();
	int setSecret();
	int deleteSecret();

	private:
	std::string server_url;
};

#endif
