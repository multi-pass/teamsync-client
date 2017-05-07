#ifndef FILEHELPER_H
#define FILEHELPER_H

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#if HAVE_CRYPTO
#include <openssl/evp.h>
#endif

class FileHelper {
	public:
	static std::string getWorkingDir();
	static bool isDir(const std::string& path);
	static bool isValidRepoPath(/* cwd */);
	static bool isValidRepoPath(const std::string& path);
	static std::string pathRelativeTo(const std::string& base_path, const std::string& path);
	static std::vector<std::string> getRecusriveFileListing(const std::string& path);
	static std::string hash_file(const std::string& filepath);

	private:
#if HAVE_CRYPTO
	static std::string libcrypto_hash_file(const std::string& digest_name,
										   const std::string& filepath);
#endif
};

#endif
