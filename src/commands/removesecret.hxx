#ifndef REMOVESECRETCOMMAND_H
#define REMOVESECRETCOMMAND_H

#include <iostream>

#include "../command.hxx"

class RemoveSecretCommand : public Command {
	public:
	RemoveSecretCommand(const std::string& working_dir);
	void run(const std::map<char, std::string> *options);
	int getResult();
};

#endif
