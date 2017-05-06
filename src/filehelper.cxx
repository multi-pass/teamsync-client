#include "filehelper.hxx"

#define STRINGIZE_DETAIL_(v) #v
#define STRINGIZE(v) STRINGIZE_DETAIL_(v)

std::string FileHelper::getWorkingDir() {
	char *_cwd = getcwd(NULL, 0);
	if (_cwd == NULL) {
		perror("getcwd()");
		return "";
	}

	std::string cwd = _cwd;
	free(_cwd);
	return cwd;
}

bool FileHelper::isValidRepoPath() {
	return FileHelper::isValidRepoPath(FileHelper::getWorkingDir());
}

bool FileHelper::isValidRepoPath(const std::string& path) {
	if (path.empty()) {
		// invalid path provided
		return false;
	}

	std::string ts_path (path + "/.ts/");
	struct stat st = {};
	if (stat(ts_path.c_str(), &st)) {
		// stat failed
		return false;
	}

	return S_ISDIR(st.st_mode);
}
