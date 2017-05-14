#ifndef SERVERFETCHCOMMAND_H
#define SERVERFETCHCOMMAND_H

#include <iostream>

#include "../filehelper.hxx"
#include "../servercommunicator.hxx"
#include "../command.hxx"

class ServerFetchCommand : public Command {
	public:
	ServerFetchCommand(const std::string& working_dir,
					   const std::string& server_url,
					   const std::string& pgpid);
	void run(const std::map<char, std::string> *options);
	int getResult();

	protected:
	ServerCommunicator comm;
	std::string cwd;
	std::string server_url;
	std::string pgpid;

	private:
	int _result;
};

#endif
