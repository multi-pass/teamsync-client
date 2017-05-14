#include "serverfetch.hxx"

void tree_callback_helper(const std::string& path, const std::string& hash_str,
						  void *userdata);

class _ServerFetchCommand_TreeTraversal : public ServerFetchCommand {
	private:
	friend void tree_callback_helper(const std::string& path,
									 const std::string& hash_str,
									 void *userdata);

	void treeCallback(const std::string& path, const std::string& hash_str);
	void updateLocalSecret(const std::string& remote_path,
						   const std::string& local_path);
};


ServerFetchCommand::ServerFetchCommand(const std::string& working_dir,
									   const std::string& server_url,
									   const std::string& pgpid)
	: cwd(working_dir), server_url(server_url), pgpid(pgpid), comm(server_url) {

}

void ServerFetchCommand::run(const std::map<char, std::string> *options) {
	this->comm.authenticate(this->pgpid);
	this->comm.getFullTree(&tree_callback_helper, (void *)this);
}

void tree_callback_helper(const std::string& path, const std::string& hash_str,
						  void *userdata) {
	_ServerFetchCommand_TreeTraversal *fetch =
		(_ServerFetchCommand_TreeTraversal *)userdata;

	fetch->treeCallback(path, hash_str);
}

void _ServerFetchCommand_TreeTraversal::updateLocalSecret(const std::string& remote_path,
														  const std::string& local_path) {
	const std::string secret_base64 = this->comm.getSecret(remote_path);

	if (!secret_base64.empty()) {
		std::ofstream out(local_path.c_str(),
						  (std::ios_base::out | std::ios_base::trunc));
		out << base64_decode(secret_base64);
		out.close();
	}
	// TODO: Else pass error to user
}

void _ServerFetchCommand_TreeTraversal::treeCallback(const std::string& path,
									  const std::string& hash_str) {
	const std::string& cwd = FileHelper::getWorkingDir();
	const std::string& local_path = (cwd + path);

	// Check if file exists
	if (!FileHelper::exists(local_path) || !FileHelper::verifyFile(local_path, hash_str)) {
		// update secret
		std::cout << "Getting secret " << path << "..." << std::endl;
		this->updateLocalSecret(path, local_path);
	}
}

int ServerFetchCommand::getResult() {
	return this->_result;
}
