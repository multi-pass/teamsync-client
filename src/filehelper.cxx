#include "filehelper.hxx"

std::string FileHelper::getWorkingDir() {
	char *_cwd = getcwd(NULL, 0);
	std::string cwd = std::string(_cwd);
	free(_cwd);
	return cwd;
}

bool FileHelper::isValidRepoPath() {
	return FileHelper::isValidRepoPath(FileHelper::getWorkingDir());
}

bool FileHelper::isValidRepoPath(const std::string& path) {
	std::string ts_path (path + "/.ts/");
	struct stat st = {};
	if (stat(ts_path.c_str(), &st)) {
		// stat failed
		return false;
	}

	return S_ISDIR(st.st_mode);
}
