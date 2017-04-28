#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <string>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

class FileHelper {
	public:
	static std::string getWorkingDir();
	static bool isValidRepoPath(/* cwd */);
	static bool isValidRepoPath(const std::string& path);
};

#endif
