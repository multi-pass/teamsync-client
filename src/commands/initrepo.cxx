#include "initrepo.hxx"

#define ARRLEN(arr) (sizeof(arr) / sizeof(*arr))

static const char *repo_folders[] = {
	"", // root folder
	"objects.cache"
};

static const char *repo_files[] = {
	"config",
	"stage"
};

InitRepoCommand::InitRepoCommand(const std::string& working_dir) {
	this->cwd = working_dir;
}

void InitRepoCommand::run() {
	const std::string repo_folder = (this->cwd + "/.ts/");

	if (FileHelper::isValidRepoPath(this->cwd)) {
		fprintf(stderr, "There is already a repo present in `%s'\n",
				repo_folder.c_str());
		this->_result = 1;
		return;
	}

	this->_result = 0;

	printf("Initializing repo in `%s'...\n", repo_folder.c_str());

	// create directories
	for (int i = 0, l = ARRLEN(repo_folders); i < l; ++i) {
		std::string folderpath = (repo_folder + repo_folders[i]);

		if (mkdir(folderpath.c_str(), 0777)) {
			perror(("mkdir(" + folderpath + ")").c_str());
			this->_result = 1;
			return;
		}
	}

	// create files
	for (int i = 0, l = ARRLEN(repo_files); i < l; ++i) {
		std::string filepath = (repo_folder + repo_files[i]);

		int conf_fd = creat(filepath.c_str(), 0666);
		if (0 > conf_fd) {
			perror(("creat(" + filepath + ")").c_str());
		} else {
			// all fine, close the file again
			close(conf_fd);
		}
	}

	if (!this->_result) {
		printf("Initialized empty TeamSync repository in %s\n",
			   repo_folder.c_str());
	} else {
		printf("Empty TeamSync repository could not be initialised in %s\n",
			   repo_folder.c_str());
	}

}

int InitRepoCommand::getResult() {
	return this->_result;
}
