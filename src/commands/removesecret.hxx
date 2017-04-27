#ifndef REMOVESECRETCOMMAND_H
#define REMOVESECRETCOMMAND_H

#include <iostream>

#include "../command.hxx"

class RemoveSecretCommand : public Command {
	public:
	RemoveSecretCommand();
	void run();
	int getResult();
};

#endif
