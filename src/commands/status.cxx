#include "status.hxx"

StatusCommand::StatusCommand(const std::string& working_dir)
	: cwd(working_dir), repo_folder(working_dir + "/.ts/"), _result(-1) {

}

void StatusCommand::run(const std::map<char, std::string> *options) {
	this->_result = 0;
}

int StatusCommand::getResult() {
	return this->_result;
}
