#include <cstring>
#include <string>
#include <list>
#include <map>
#include <vector>

#include <unistd.h>
#include <getopt.h>

enum OptionArgument {
	none = 0,
	required = 1,
	optional = 2
};

class OptionsResult {
	public:
	OptionsResult(std::map<char, std::string> *options,
				  std::list<char> *missing_arguments,
				  std::list<std::string> *unknown_options,
				  std::list<std::string> *non_option_arguments);
	~OptionsResult();

	const std::map<char, std::string> * const options;
	const std::list<char> * const options_with_missing_arguments;
	const std::list<std::string> * const unknown_options;
	const std::list<std::string> * const non_option_arguments;
};

class OptionsParser {
	public:
	OptionsParser(unsigned argc, char *argv[]);
	void setOptstring(const char *optstring);
	void addLongMapping(const char *longopt, char opt, OptionArgument has_arg);
	void removeLongMapping(const char *longopt);

	OptionsResult parse();

	private:
	unsigned argc;
	char **argv;
	const char *optstring;
	std::vector<struct option> longmap;
};
