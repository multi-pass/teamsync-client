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

std::string FileHelper::hash_file(const std::string& filepath) {
#if HAVE_CRYPTO
	// Use OpenSSL's libcrypto to calculate file hashes
#if CRYPTO_WITH_SHA256
	const std::string dgst_algo = "sha256";
#elif CRYPTO_WITH_RIPEMD
	const std::string dgst_algo = "ripemd160";
#elif CRYPTO_WITH_SHA
	const std::string dgst_algo = "sha1";
#else
#error libcrypto supports none of the supported algorithms
#endif

	std::string hash_str = FileHelper::libcrypto_hash_file(dgst_algo, filepath);

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
std::string FileHelper::libcrypto_hash_file(const std::string& digest_name,
											const std::string& filepath) {
	const EVP_MD *dgst_type = EVP_get_digestbyname(digest_name.c_str());
	if (EVP_add_digest(dgst_type)) {
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
