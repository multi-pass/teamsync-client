#ifndef SERVERFETCHCOMMAND_H
#define SERVERFETCHCOMMAND_H

#include <iostream>

#include "../command.hxx"

class ServerFetchCommand : public Command {
	public:
	ServerFetchCommand(const std::string& working_dir);
	void run(const std::map<char, std::string> *options);
	int getResult();
};

#endif
