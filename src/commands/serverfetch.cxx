#include "serverfetch.hxx"

ServerFetchCommand::ServerFetchCommand(const std::string& working_dir,
									   const std::string& server_url,
									   const std::string& pgpid)
	: cwd(working_dir), server_url(server_url), pgpid(pgpid) {

}

void ServerFetchCommand::run(const std::map<char, std::string> *options) {
	std::cout << "Running server fetch command" << std::endl;
}

int ServerFetchCommand::getResult() {

}
