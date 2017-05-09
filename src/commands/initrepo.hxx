#ifndef INITREPOCOMMAND_H
#define INITREPOCOMMAND_H

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../command.hxx"
#include "../filehelper.hxx"

class InitRepoCommand : public Command {
	public:
	InitRepoCommand(const std::string& working_dir);
	void run(const std::map<char, std::string> *options);
	int getResult();

	private:
	int _result;
	std::string cwd;
};

#endif
