#ifndef INITREPOCOMMAND_H
#define INITREPOCOMMAND_H

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../command.hxx"
#include "../filehelper.hxx"

class InitRepoCommand : public Command {
	public:
	InitRepoCommand();
	void run();
	int getResult();

	private:
	int _result;
	std::string cwd;
};

#endif
