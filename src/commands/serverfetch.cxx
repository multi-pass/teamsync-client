#include "serverfetch.hxx"

void tree_callback(const std::string& path, const std::string& hash_str);

ServerFetchCommand::ServerFetchCommand(const std::string& working_dir,
									   const std::string& server_url,
									   const std::string& pgpid)
	: cwd(working_dir), server_url(server_url), pgpid(pgpid) {

}

void ServerFetchCommand::run(const std::map<char, std::string> *options) {
	ServerCommunicator comm(this->server_url);
	comm.authenticate(this->pgpid);
	comm.getFullTree(&tree_callback);
}

void tree_callback(const std::string& path, const std::string& hash_str) {
	printf("%s (%s)\n", path.c_str(), hash_str.c_str());
}

int ServerFetchCommand::getResult() {
	return this->_result;
}
