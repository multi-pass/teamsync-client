#ifndef CLICONTROLLER_H
#define CLICONTROLLER_H

#include <stdio.h>

#include "optionsparser.hxx"

class CLIController {
	public:
	static int dispatch(int argc, char **argv);
};

#endif
