#ifndef SERVERPUSHCOMMAND_H
#define SERVERPUSHCOMMAND_H

#include <iostream>

#include "../command.hxx"

class ServerPushCommand : public Command {
	public:
	ServerPushCommand(const std::string& working_dir);
	void run();
	int getResult();
};

#endif
