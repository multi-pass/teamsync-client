#ifndef ADDSECRETCOMMAND_H
#define ADDSECRETCOMMAND_H

#include <iostream>

#include "../command.hxx"

class AddSecretCommand : public Command {
	public:
	AddSecretCommand();
	void run();
	int getResult();
};

#endif
