#ifndef PGPHELPER_H
#define PGPHELPER_H

#include <cstring>

#include <gpgme.h>


class PGPHelper {
	public:
	PGPHelper(const char *filepath);
	~PGPHelper();

	private:
	void processFile();
	const char *filepath;
}

#endif
