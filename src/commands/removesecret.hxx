#ifndef REMOVESECRETCOMMAND_H
#define REMOVESECRETCOMMAND_H

#include <iostream>

#include "../command.hxx"

class RemoveSecretCommand : public Command {
	public:
	RemoveSecretCommand(const std::string& working_dir);
	void run(const OptionsResult& options);
	int getResult();
};

#endif
