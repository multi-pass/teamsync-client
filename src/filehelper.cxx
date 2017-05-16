#include "filehelper.hxx"

#define STRINGIZE_DETAIL_(v) #v
#define STRINGIZE(v) STRINGIZE_DETAIL_(v)

const std::string FileHelper::getWorkingDir() {
	char *_cwd = getcwd(NULL, 0);
	if (_cwd == NULL) {
		perror("getcwd()");
		return "";
	}

	std::string cwd = _cwd;
	free(_cwd);
	return cwd;
}

bool FileHelper::exists(const std::string& path) {
	struct stat st;
	return (stat(path.c_str(), &st) == 0);
}

bool FileHelper::isDir(const std::string& path) {
	struct stat st;
	if (stat(path.c_str(), &st)) {
		// stat failed
		return false;
	}

	return S_ISDIR(st.st_mode);
}

bool FileHelper::isValidRepoPath() {
	return FileHelper::isValidRepoPath(FileHelper::getWorkingDir());
}

bool FileHelper::isValidRepoPath(const std::string& path) {
	if (path.empty()) {
		// invalid path provided
		return false;
	}

	std::string ts_path(path + "/.ts/");
	return FileHelper::isDir(ts_path);
}

// FIXME: This implementation is probably the ugliest one to find.
// It needed to be done fast, but wants to be rewritten!
const std::string FileHelper::pathRelativeTo(const std::string& base_path,
									   const std::string& path) {
	std::vector<std::string> base_components, path_components;
	std::string filename;

	{
		std::stringstream ss;
		ss.str(base_path);

		std::string pcomp;
		while (std::getline(ss, pcomp, '/')) {
			base_components.insert(base_components.end(), pcomp);
		}

		if (!base_components.empty()) {
			if (!FileHelper::isDir(base_path)) {
				base_components.pop_back();
			}
			if (base_components.front().empty()) {
				base_components.erase(base_components.begin());
			}
		}
	}
	{
		std::stringstream ss;
		ss.str(path);

		std::string pcomp;
		while (std::getline(ss, pcomp, '/')) {
			path_components.insert(path_components.end(), pcomp);
		}
		if (!path_components.empty()) {
			if (!FileHelper::isDir(path)) {
				filename = path_components.back();
				path_components.pop_back();
			}

			if (path_components.front().empty()) {
				path_components.erase(path_components.begin());
			}
		}
	}

	if (base_components.empty()) { return path; }

	// trim common parts from the beginning
	while (!base_components.empty() && !path_components.empty()
		   && base_components.front() == path_components.front()) {
		base_components.erase(base_components.begin());
		path_components.erase(path_components.begin());
	}

	// insert "go up" components
	std::vector<std::string>::size_type base_comp_count = base_components.size();

	std::string relpath(".");
	{
		std::ostringstream os;
		for (int i = 0; i < base_comp_count; ++i) { os << "/.."; }

		relpath += (os.str() + "/");
	}

	// append rest of path
	{
		std::ostringstream os;
		std::copy(path_components.begin(), path_components.end(),
				  std::ostream_iterator<std::string>(os, "/"));

		relpath += os.str();
	}

	if (!filename.empty()) {
		relpath += filename;
	}

	return relpath;
}

bool listdir(const std::string& path, std::vector<std::string>& list) {
	bool errors = false;

	// ensure path ends with a slash
	std::string in_path(path);
	if (path.compare((path.length() - 1), 1, "/")) {
		in_path += "/";
	}

	struct dirent *entry;
	DIR *dp;

	dp = opendir(in_path.c_str());

	if (dp == NULL) {
		perror(("opendir(" + in_path + ")").c_str());
		return false;
	}

	std::vector<std::string> subdirs;

	while ((entry = readdir(dp))) {
		std::string d_name(entry->d_name);
		std::string d_path(in_path + d_name);

		if ("." == d_name || ".." == d_name) { continue; }

		if (FileHelper::isDir(d_path)) {
			if (!d_name.compare(0, 3, ".ts")) {
				// ignore repo files
				continue;
			}

			subdirs.insert(subdirs.end(), (d_path + "/"));
		} else {
			// add element to file list
			list.insert(list.end(), d_path);
		}
	}

	closedir(dp);

	// go through subdirs
	for (std::vector<std::string>::iterator it = subdirs.begin(),
			 end = subdirs.end(); it != end; ++it) {
		errors |= !listdir(*it, list);
	}

	return !errors;
}


std::vector<std::string> FileHelper::getRecursiveFileListing(const std::string& path) {
	std::vector<std::string> list;
	listdir(path, list);
	return list;
}

const std::string FileHelper::hash_file(const std::string& filepath) {
#if HAVE_CRYPTO
	// Use OpenSSL's libcrypto to calculate file hashes
#if CRYPTO_WITH_SHA256
	const std::string dgst_algo("sha256");
#elif CRYPTO_WITH_RIPEMD
	const std::string dgst_algo("ripemd160");
#elif CRYPTO_WITH_SHA
	const std::string dgst_algo("sha1");
#else
#error libcrypto supports none of the supported algorithms
#endif

	// Call libcrypto_hash_file
	std::string hash_str(FileHelper::libcrypto_hash_file(dgst_algo, filepath));

	return (!hash_str.empty()
			? (dgst_algo + ":" + hash_str)
			: "");
#else
/** Note:
 * At the moment only OpenSSL's libcrypto is supported for calculating file
 * hashes.
 * Feel free to submit a pull request that adds more libraries.
 */
#error no library for hashing found
#endif
}


#if HAVE_CRYPTO
const std::string FileHelper::libcrypto_hash_file(const std::string& digest_name,
											const std::string& filepath) {
	static bool dgst_loaded = false;
	if (!dgst_loaded) {
#if CRYPTO_WITH_SHA256
		EVP_add_digest(EVP_sha256());
#endif
#if CRYPTO_WITH_RIPEMD
		EVP_add_digest(EVP_ripemd160());
#endif
#if CRYPTO_WITH_SHA
		EVP_add_digest(EVP_sha1());
#endif
		dgst_loaded = true;
	}

	const EVP_MD *dgst_type = EVP_get_digestbyname(digest_name.c_str());
	if (!dgst_type) {
		return "";
	}

	unsigned char hash_str[EVP_MAX_MD_SIZE];
	unsigned int hash_len = 0;
	std::stringstream oss;

	EVP_MD_CTX *ctx = EVP_MD_CTX_create();

	// Open file
	FILE *file = fopen(filepath.c_str(), "r");
	if (!file) {
		perror(("fopen(" + filepath + ")").c_str());
		goto bailout;
	}

	EVP_DigestInit_ex(ctx, dgst_type, NULL);

	// Read file into digest update
	{
		void *buf = malloc(BUFSIZ);
		if (!buf) {
			perror(("malloc(" STRINGIZE(BUFSIZ) ")"));
			goto bailout;
		}

		size_t bytes_read = 0;
		while ((bytes_read = fread(buf, 1, BUFSIZ, file))) {
			EVP_DigestUpdate(ctx, buf, bytes_read);
		}

		free(buf);
	}

	// Extract hash
	EVP_DigestFinal_ex(ctx, hash_str, &hash_len);
	EVP_MD_CTX_cleanup(ctx);

	for (unsigned int i = 0; i < hash_len; ++i) {
		oss << std::hex << (int)hash_str[i];
	}

	return oss.str();

  bailout:
	if (ctx) {
		EVP_MD_CTX_cleanup(ctx);
	}
	return "";
}
#endif

bool FileHelper::verifyFile(const std::string& path,
							const std::string& hash_str) {
	std::stringstream ss(hash_str);
	std::string dgst_algo;
	std::string hash;
	std::getline(ss, dgst_algo, ss.widen(':'));
	std::getline(ss, hash);

	std::string file_hash(FileHelper::libcrypto_hash_file(dgst_algo, path));

	return (hash == file_hash);
}
