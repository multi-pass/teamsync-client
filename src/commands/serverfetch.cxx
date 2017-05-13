#include "serverfetch.hxx"

ServerFetchCommand::ServerFetchCommand(const std::string& working_dir,
									   const std::string& server_url,
									   const std::string& pgpid)
	: cwd(working_dir), server_url(server_url), pgpid(pgpid) {

}

void ServerFetchCommand::run(const std::map<char, std::string> *options) {
	ServerCommunicator comm(this->server_url);
	comm.authenticate(this->pgpid);
}

int ServerFetchCommand::getResult() {
	return this->_result;
}
