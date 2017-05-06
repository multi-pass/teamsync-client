#ifndef STATUSCOMMAND_H
#define STATUSCOMMAND_H

#include <string>

#include "../command.hxx"

class StatusCommand : public Command {
	public:
	StatusCommand(const std::string& working_dir);
	void run();
	int getResult();

	private:
	int _result;
	std::string cwd;
	std::string repo_folder;
};

#endif
