#include "pgphelper.hxx"

PGPHelper::GPGHelper(const char *filepath) {
	this->filepath = filepath;

	this->processFile();
}

PGPHelper::processFile() {
	gpgme_ctx_t ctx;  // the context
	gpgme_error_t err; // errors
	gpgme_key_t key; // the key
	gpgme_keylist_result_t result; // the keylist results

	// initialize gpgme
	gpgme_check_version(NULL);

	// initialize context
	gpgme_new(&ctx);

	gpgme_ctx_set_engine_info(ctx, GPGME_PROTOCOL_OpenPGP, NULL, "/dev/null");

	gpgme_op_keylist_start(ctx, NULL, 0); // start the keylist

	while (!(err = gpgme_op_keylist_next(ctx, &key))) { // loop through the keys in the keyring
		fprintf(stdout, "Key ID: %s\n", key->subkeys->keyid); // print out the keyid
		gpgme_key_unref(key); // release the key reference
	}

	gpgme_release(ctx); // release the context, all done
}
