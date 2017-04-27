#ifndef SERVERPUSHCOMMAND_H
#define SERVERPUSHCOMMAND_H

#include <iostream>

#include "../command.hxx"

class ServerPushCommand : public Command {
	public:
	ServerPushCommand();
	void run();
	int getResult();
};

#endif
