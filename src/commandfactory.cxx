#include "commandfactory.hxx"

enum CommandType {
	INVALID_COMMAND = 0,
	ADD_SECRET_COMMAND = 1,
	REMOVE_SECRET_COMMAND,
	SERVER_FETCH_COMMAND,
	SERVER_PUSH_COMMAND
};

std::map<std::string, CommandType> define_aliases() {
	std::map<std::string, CommandType> aliases;
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
	// Resolve alias
	CommandType type = INVALID_COMMAND;
	{
		std::map<std::string, CommandType>::iterator type_it =
			aliases.find(command_string);
		if (type_it != aliases.end()) {
			type = type_it->second;
		}
	}

	// Create command instance
	switch (type) {
	case ADD_SECRET_COMMAND:
		*command = new AddSecretCommand();
		break;
	case REMOVE_SECRET_COMMAND:
		*command = new RemoveSecretCommand();
		break;
	case SERVER_FETCH_COMMAND:
		*command = new ServerFetchCommand();
		break;
	case SERVER_PUSH_COMMAND:
		*command = new ServerPushCommand();
		break;

	case INVALID_COMMAND:
	default:
		fprintf(stderr, "You specified an invalid command\n");
		*command = NULL;
	}
}
