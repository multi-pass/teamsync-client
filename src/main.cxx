#if HAVE_LIBCURL
#include <curl/curl.h>
#endif

#include "clicontroller.hxx"

const char *DOLLAR_ZERO;

int main(int argc, char *argv[]) {
	DOLLAR_ZERO = argv[0];

#if HAVE_LIBCURL
	curl_global_init(CURL_GLOBAL_DEFAULT);
#endif

	return CLIController::dispatch(argc, argv);
}
