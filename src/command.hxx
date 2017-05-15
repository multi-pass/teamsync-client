#ifndef COMMAND_H
#define COMMAND_H

#include <map>
#include <string>

#include "optionsparser.hxx"

class Command {
	public:
	virtual void run(const OptionsResult& options) = 0;
	virtual int getResult() = 0;

	virtual ~Command() {};
};

#endif
