#include "commandfactory.hxx"

enum CommandType {
	INVALID_COMMAND = 0,
	INIT_COMMAND = 1,
	ADD_SECRET_COMMAND,
	REMOVE_SECRET_COMMAND,
	SERVER_FETCH_COMMAND,
	SERVER_PUSH_COMMAND
};

std::map<std::string, CommandType> define_aliases() {
	std::map<std::string, CommandType> aliases;
	aliases.insert(std::make_pair("init", INIT_COMMAND));
	aliases.insert(std::make_pair("add", ADD_SECRET_COMMAND));
	aliases.insert(std::make_pair("remove", REMOVE_SECRET_COMMAND));
	aliases.insert(std::make_pair("stage", ADD_SECRET_COMMAND));
	aliases.insert(std::make_pair("unstage", REMOVE_SECRET_COMMAND));
	aliases.insert(std::make_pair("push", SERVER_PUSH_COMMAND));
	aliases.insert(std::make_pair("fetch", SERVER_FETCH_COMMAND));
	return aliases;
}
std::map<std::string, CommandType> aliases = define_aliases();


void CommandFactory::create(const std::string command_string, Command **command) {
	// Set command to NULL as a fallback, in case there is a left-over object
	// or the variable has not been initialized yet.
	*command = NULL;


	// Resolve aliases
	CommandType type = INVALID_COMMAND;
	{
		std::map<std::string, CommandType>::iterator type_it =
			aliases.find(command_string);
		if (type_it != aliases.end()) {
			type = type_it->second;
		}
	}

	// Check if the current working directory is a repo
	const std::string cwd = FileHelper::getWorkingDir();
	bool is_repo = FileHelper::isValidRepoPath(cwd);


	// Create command instance

	if (INVALID_COMMAND == type) {
		// user specified an invalid command
		fprintf(stderr, "`'%s' is not a valid command\n"
				"Use `--help' to get a list of valid commands.\n",
				command_string.c_str());
		goto postprocessing;
	}

	// Commands that require a repo to be present
	if (is_repo) {
		switch (type) {
		case ADD_SECRET_COMMAND:
			*command = new AddSecretCommand(cwd);
			goto postprocessing;
		case REMOVE_SECRET_COMMAND:
			*command = new RemoveSecretCommand(cwd);
			goto postprocessing;
		case SERVER_FETCH_COMMAND:
			*command = new ServerFetchCommand(cwd);
			goto postprocessing;
		case SERVER_PUSH_COMMAND:
			*command = new ServerPushCommand(cwd);
			goto postprocessing;
		}
	}

	// Commands that work without a repo
	switch (type) {
	case INIT_COMMAND:
		*command = new InitRepoCommand(cwd);
		goto postprocessing;
	}

	// No command match found and no repo probably means that the user provided
	// a command that requires a repo to be present.
	if (!is_repo) {
		fprintf(stderr, "Not a ts repo.\n");
	}

	postprocessing:
	return;
}
