#include "serverpush.hxx"

ServerPushCommand::ServerPushCommand(const std::string& working_dir,
									 const std::string& server_url,
									 const std::string& pgpid)
	: cwd(working_dir), server_url(server_url), pgpid(pgpid), comm(server_url) {

}

void ServerPushCommand::run(const OptionsResult& options) {
	this->comm.authenticate(this->pgpid);

	const std::string path("/secret1"),
		local_path(this->cwd + path);

	std::ifstream in(local_path.c_str());
	std::string payload((std::istreambuf_iterator<char>(in)),
						 std::istreambuf_iterator<char>());


	this->_result = !(200 == this->comm.setSecret(path, payload));
}

int ServerPushCommand::getResult() {
	return this->_result;
}
