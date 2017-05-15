#ifndef SERVERPUSHCOMMAND_H
#define SERVERPUSHCOMMAND_H

#include <fstream>
#include <iostream>

#include <limits.h>
#include <stdlib.h>

#include "../filehelper.hxx"
#include "../servercommunicator.hxx"
#include "../command.hxx"

class ServerPushCommand : public Command {
	public:
	ServerPushCommand(const std::string& working_dir,
					  const std::string& server_url,
					  const std::string& pgpid);

	void run(const OptionsResult& options);
	int getResult();

	private:
	ServerCommunicator comm;
	const std::string cwd;
	const std::string server_url;
	const std::string pgpid;

	int _result;
};

#endif
