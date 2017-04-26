#ifndef COMMAND_H
#define COMMAND_H

class Command {
	public:
	virtual void run() = 0;
	virtual int getResult() = 0;

	virtual ~Command() {};
};

#endif
