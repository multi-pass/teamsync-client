#ifndef CLICONTROLLER_H
#define CLICONTROLLER_H

#if HAVE_CONFIG_H
#include "../config.h"
#endif


#include <stdio.h>
#include <unistd.h>

#include "commandfactory.hxx"
#include "optionsparser.hxx"

class CLIController {
	public:
	static int dispatch(int argc, char **argv);
};

#endif
