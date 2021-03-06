#include "commandfactory.hxx"

enum CommandType {
	INVALID_COMMAND = 0,
	INIT_COMMAND = 1,
	STATUS_COMMAND,
	ADD_SECRET_COMMAND,
	REMOVE_SECRET_COMMAND,
	SERVER_FETCH_COMMAND,
	SERVER_PUSH_COMMAND,
	SERVER_SYNC_COMMAND
};

std::map<std::string, CommandType> define_aliases() {
	std::map<std::string, CommandType> aliases;
	aliases.insert(std::make_pair("init", INIT_COMMAND));
	aliases.insert(std::make_pair("status", STATUS_COMMAND));
	aliases.insert(std::make_pair("add", ADD_SECRET_COMMAND));
	aliases.insert(std::make_pair("remove", REMOVE_SECRET_COMMAND));
	aliases.insert(std::make_pair("stage", ADD_SECRET_COMMAND));
	aliases.insert(std::make_pair("unstage", REMOVE_SECRET_COMMAND));
	aliases.insert(std::make_pair("push", SERVER_PUSH_COMMAND));
	aliases.insert(std::make_pair("fetch", SERVER_FETCH_COMMAND));
	aliases.insert(std::make_pair("sync", SERVER_SYNC_COMMAND));
	return aliases;
}

static const std::map<std::string, CommandType> aliases(define_aliases());
static AppConfig appconfig((FileHelper::getWorkingDir() + "/.ts/config.json"));



void CommandFactory::create(const std::string command_string, Command **command) {
	// Set command to NULL as a fallback, in case there is a left-over object
	// or the variable has not been initialized yet.
	*command = NULL;


	// Resolve aliases
	CommandType type = INVALID_COMMAND;
	{
		std::map<std::string, CommandType>::const_iterator type_it =
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
		case STATUS_COMMAND:
			*command = new StatusCommand(cwd);
			goto postprocessing;
		case ADD_SECRET_COMMAND:
			*command = new AddSecretCommand(cwd);
			goto postprocessing;
		case REMOVE_SECRET_COMMAND:
			*command = new RemoveSecretCommand(cwd);
			goto postprocessing;
		case SERVER_FETCH_COMMAND: {
			const std::string& conf_server = appconfig.getString("server_url");
			const std::string& conf_fingerprint = appconfig.getString("fingerprint");
			*command = new ServerFetchCommand(cwd, conf_server, conf_fingerprint);
			goto postprocessing;
		}
		case SERVER_PUSH_COMMAND: {
			const std::string& conf_server = appconfig.getString("server_url");
			const std::string& conf_fingerprint = appconfig.getString("fingerprint");
			*command = new ServerPushCommand(cwd, conf_server, conf_fingerprint);
			goto postprocessing;
		}
		case SERVER_SYNC_COMMAND:
			*command = new ServerSyncCommand(cwd);
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
