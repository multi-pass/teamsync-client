#ifndef ADDSECRETCOMMAND_H
#define ADDSECRETCOMMAND_H

#include <iostream>

#include "../command.hxx"

class AddSecretCommand : public Command {
	public:
	AddSecretCommand(const std::string& working_dir);
	void run(const std::map<char, std::string> *options);
	int getResult();
};

#endif
