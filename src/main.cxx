#include "clicontroller.hxx"

const char *DOLLAR_ZERO;

int main(int argc, char *argv[]) {
	DOLLAR_ZERO = argv[0];

	return CLIController::dispatch(argc, argv);
}
