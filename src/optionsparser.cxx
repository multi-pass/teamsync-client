#include "optionsparser.hxx"
#include <stdio.h>
OptionsParser::OptionsParser(unsigned int _argc, char **_argv)
	: argc(_argc), argv(_argv), longmap() {
	this->longmap.insert(this->longmap.end(), option());
}

void OptionsParser::setOptstring(const char *optstring) {
	// we prepend a colon to the optstring, to make getopt return ':' instead
	// of '?' when an option is missing an argument.

	char *_str = new char[(std::strlen(optstring)+1)];
	_str[0] = ':';
	std::strcpy((_str+1), optstring);
	this->optstring = _str;
}

void OptionsParser::addLongMapping(const char *longopt, char opt,
								   OptionArgument has_arg) {
	option _opt = {};
	_opt.name = longopt;
	_opt.has_arg = has_arg;
	_opt.val = opt;

	this->longmap.insert(--this->longmap.end(), _opt);
}

void OptionsParser::removeLongMapping(const char *longopt) {
	for (std::vector<option>::iterator it = this->longmap.begin(),
			 end = this->longmap.end(); it != end; ++it) {
		if (!std::strcmp(it->name, longopt)) {
			this->longmap.erase(it);
			break;
		}
	}
}

OptionsResult OptionsParser::parse() {
	extern char *optarg;
	extern int optind, opterr, optopt;

	opterr = 0;
	int c, longindex = 0;

	// Data structures (are going to be parred to OptionsResult object)
	std::map<char, std::string> *options = new std::map<char, std::string>();
	std::list<char> *missing_arguments = new std::list<char>();
	std::list<std::string> *unknown_options = new std::list<std::string>(),
		*non_option_arguments = new std::list<std::string>();

	option *longopts = (this->longmap.size() ? &(this->longmap.front()) : NULL);

	// Parse options
	while (-1 != (c = getopt_long(this->argc, this->argv, this->optstring,
							longopts, &longindex))) {
		bool valid_opt = (c != '?' && c != ':' && std::strchr(optstring, optopt));
		if (!valid_opt || c == '?' || c == ':') {
			if (c == ':') {
				missing_arguments->insert(missing_arguments->end(), optopt);
			} else {
				unknown_options->insert(unknown_options->end(),
										(longindex
										 ? std::string(this->longmap[longindex].name)
										 : std::string(1, optopt)));
			}
		} else {
			// valid option
			options->insert(std::make_pair(c, (optarg
											  ? std::string(optarg)
											  : "")));
		}
	}

	// The rest are non-option arguments
	for (int i = optind, c = this->argc; i < c; ++i) {
		non_option_arguments->insert(non_option_arguments->end(),
									std::string(this->argv[i]));
	}

	return OptionsResult(options, missing_arguments, unknown_options, non_option_arguments);
}



OptionsResult::OptionsResult(std::map<char, std::string> *_options,
							 std::list<char> *_missing_arguments,
							 std::list<std::string> *_unknown_options,
							 std::list<std::string> *_non_option_arguments)
	: options(_options),
	  options_with_missing_arguments(_missing_arguments),
	  unknown_options(_unknown_options),
	  non_option_arguments(_non_option_arguments) {
}

OptionsResult::~OptionsResult() {
	delete this->options;
	delete this->options_with_missing_arguments;
	delete this->unknown_options;
	delete this->non_option_arguments;
}
