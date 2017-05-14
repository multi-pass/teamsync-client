#include "serverfetch.hxx"

static void tree_callback(const std::string& path, const std::string& hash_str);

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

static void tree_callback(const std::string& path,
						  const std::string& hash_str) {
	const std::string& cwd = FileHelper::getWorkingDir();
	const std::string& local_path = (cwd + path);


	// Check if file exists
	if (FileHelper::exists(local_path)) {
		// Check its hash
		if (!FileHelper::verifyFile(local_path, hash_str)) {
			std::cout << "Hashes dont match" << std::endl;
		}
	} else {
		std::cout << "File " << path << " is missing locally" << std::endl;
	}
}

int ServerFetchCommand::getResult() {
	return this->_result;
}
