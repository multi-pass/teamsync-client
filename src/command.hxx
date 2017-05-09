#ifndef COMMAND_H
#define COMMAND_H

#include <map>
#include <string>

class Command {
	public:
	virtual void run(const std::map<char, std::string> *options) = 0;
	virtual int getResult() = 0;

	virtual ~Command() {};
};

#endif
