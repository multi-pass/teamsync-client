#ifndef CLICONTROLLER_H
#define CLICONTROLLER_H

#include <stdio.h>

#if HAVE_CONFIG_H
#include "../config.h"
#endif

#include "commandfactory.hxx"
#include "optionsparser.hxx"

class CLIController {
	public:
	static int dispatch(int argc, char **argv);
};

#endif
