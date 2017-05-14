#ifndef SERVERPUSHCOMMAND_H
#define SERVERPUSHCOMMAND_H

#include <fstream>
#include <iostream>


#include "../servercommunicator.hxx"
#include "../command.hxx"

class ServerPushCommand : public Command {
	public:
	ServerPushCommand(const std::string& working_dir,
					  const std::string& server_url,
					  const std::string& pgpid);

	void run(const std::map<char, std::string> *options);
	int getResult();

	private:
	ServerCommunicator comm;
	std::string cwd;
	std::string server_url;
	std::string pgpid;

	int _result;
};

#endif
