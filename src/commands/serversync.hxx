#ifndef SERVERSYNCCOMMAND_H
#define SERVERSYNCCOMMAND_H

#include <iostream>

#include "../command.hxx"

class ServerSyncCommand : public Command {
	public:
	ServerSyncCommand(const std::string& working_dir);
	void run(const OptionsResult& options);
	int getResult();
};

#endif
