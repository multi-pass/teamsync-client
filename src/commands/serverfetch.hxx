#ifndef SERVERFETCHCOMMAND_H
#define SERVERFETCHCOMMAND_H

#include <fstream>
#include <iostream>

#include "../base64.hxx"
#include "../filehelper.hxx"
#include "../servercommunicator.hxx"
#include "../command.hxx"

class ServerFetchCommand : public Command {
	public:
	ServerFetchCommand(const std::string& working_dir,
					   const std::string& server_url,
					   const std::string& fingerprint);

	void run(const OptionsResult& options);
	int getResult();

	protected:
	ServerCommunicator comm;
	const std::string cwd;
	const std::string server_url;
	const std::string fingerprint;
	int _result;
};

#endif
