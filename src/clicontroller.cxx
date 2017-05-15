#include "clicontroller.hxx"

void print_version() {
	printf("%s, version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
}

void print_usage() {
	extern const char *DOLLAR_ZERO;
	printf("Usage: %s [OPTION]...\n", DOLLAR_ZERO);
}

void print_help() {
	printf(PACKAGE_NAME " " PACKAGE_VERSION "\n"
		   "TeamSync Client application\n\n");

	print_usage();

	printf("\nOptions: \n"
		   "  -h, --help    Print help\n"
		   "  --usage       Print usage guide\n"
		   "  -V            Print version.\n"
		   "\n");
}


int CLIController::dispatch(int argc, char *argv[]) {
	OptionsParser opts_parser(argc, argv);
	opts_parser.setOptstring("C:hV");
	opts_parser.addLongMapping("chdir", 'C', required);
	opts_parser.addLongMapping("help", 'h', none);
	opts_parser.addLongMapping("usage", 'u', none);
	opts_parser.addLongMapping("version", 'V', none);
	OptionsResult args = opts_parser.parse();


	for (std::list<std::string>::const_iterator it = args.unknown_options->begin(),
			 end = args.unknown_options->end(); it != end; ++it) {
		fprintf(stderr, "Unknown option -%s read\n", it->c_str());
	}

	for (std::list<char>::const_iterator it = args.options_with_missing_arguments->begin(),
			 end = args.options_with_missing_arguments->end(); it != end; ++it) {
		fprintf(stderr, "Option -%c needs an argument\n", *it);
	}


	for (std::map<char, std::string>::const_iterator it = args.options->begin(),
			 end = args.options->end(); it != end; ++it) {
		switch (it->first) {
		case 'C': {
			if (chdir(it->second.c_str())) {
				perror(("chdir(" + it->second + ")").c_str());
				return 1;
			}
			break;
		}
		case 'h':
			print_help();
			return 0;
		case 'u':
			print_usage();
			return 0;
		case 'V':
			print_version();
			return 0;
		}
	}


	// Get primary operation
	if (!args.non_option_arguments->size()) {
		fprintf(stderr, "Please specify a command\n");
		return 1;
	}

	std::string op = args.non_option_arguments->front();
	((std::list<std::string> *)args.non_option_arguments)->pop_front();

	Command *cmd = NULL;
	CommandFactory::create(op, &cmd);

	if (cmd) {
		cmd->run(args);

		int res = cmd->getResult();
		delete cmd;
		return res;
	}
	return 1;
}
