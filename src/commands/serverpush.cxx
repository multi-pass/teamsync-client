#include "serverpush.hxx"

ServerPushCommand::ServerPushCommand(const std::string& working_dir,
									 const std::string& server_url,
									 const std::string& fingerprint)
	: cwd(working_dir), server_url(server_url), fingerprint(fingerprint),
	comm(server_url) {

}

void ServerPushCommand::run(const OptionsResult& options) {
	const std::list<std::string>& non_option_arguments = *(options.non_option_arguments);
	if (non_option_arguments.empty()) {
		fprintf(stderr, "Please provide a file to push.\n");
		this->_result = 1;
		return;
	}

	std::string local_path, repo_path;
	{
		const std::string path(non_option_arguments.front());
		char *rpath;

		if (!(rpath = realpath(path.c_str(), NULL))) {
			// File does not exist
			perror(path.c_str());
			this->_result = 1;
			return;
		}

		local_path = rpath;
		free(rpath);

		repo_path = FileHelper::pathRelativeTo(this->cwd, local_path);
		repo_path.erase(repo_path.begin()); // remove leading '.'
	}

#ifdef DEBUG
	printf("Pushing %s...\n", repo_path.c_str());
#endif

	std::ifstream in(local_path.c_str());
	if (!in.is_open()) {
		fprintf(stderr, "Could not open %s.\n", local_path.c_str());
		this->_result = 1;
		return;
	}

	std::string payload((std::istreambuf_iterator<char>(in)),
						std::istreambuf_iterator<char>());

	// Communicate with server
	this->comm.authenticate(this->fingerprint);
	this->_result = !(200 == this->comm.setSecret(repo_path, payload));
}

int ServerPushCommand::getResult() {
	return this->_result;
}
