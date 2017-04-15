#include <stdio.h>
#include "optionsparser.hxx"
static char *DOLLAR_ZERO;

void print_version() {
	printf("%s, version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
}

void print_usage() {
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


int main(int argc, char *argv[]) {
	DOLLAR_ZERO = argv[0];

	OptionsParser opts_parser(argc, argv);
	opts_parser.setOptstring("hV");
	opts_parser.addLongMapping("help", 'h', none);
	opts_parser.addLongMapping("usage", 'u', none);
	OptionsResult args = opts_parser.parse();

	for (std::list<std::string>::const_iterator it = args.unknown_options->begin();
		 it != args.unknown_options->end(); ++it) {
		fprintf(stderr, "Unknown option -%s read\n", it->c_str());
	}

	fprintf(stderr, "Missing arguments:\n");
	for (std::list<char>::const_iterator it = args.options_with_missing_arguments->begin();
		 it != args.options_with_missing_arguments->end(); ++it) {
		fprintf(stderr, "Option -%c needs an argument\n", *it);
	}

	for (std::map<char, std::string>::const_iterator it = args.options->begin();
		 it != args.options->end(); ++it) {
		switch (it->first) {
		case 'h':
			print_help();
			break;
		case 'u':
			print_usage();
			break;
		case 'V':
			print_version();
			break;
		}
	}

	return 0;
}
