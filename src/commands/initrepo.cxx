#include "initrepo.hxx"

InitRepoCommand::InitRepoCommand() {
	this->cwd = FileHelper::getWorkingDir();
}

void InitRepoCommand::run() {
	const std::string repo_folder = (this->cwd + "/.ts/");
	if (FileHelper::isValidRepoPath(this->cwd)) {
		fprintf(stderr, "There is already a repo present in `%s'\n",
				repo_folder.c_str());
		this->_result = 1;
		return;
	}

	printf("Initializing repo in `%s'...\n", repo_folder.c_str());

	mkdir(repo_folder.c_str(), 0777);

	this->_result = 0;
}

int InitRepoCommand::getResult() {
	return this->_result;
}
