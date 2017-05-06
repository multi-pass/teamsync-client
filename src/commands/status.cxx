#include "status.hxx"

StatusCommand::StatusCommand(const std::string& working_dir) {
	this->cwd = working_dir;
	this->repo_folder = (this->cwd + "/.ts/");
}

void StatusCommand::run() {
	this->_result = 0;
}

int StatusCommand::getResult() {
	return this->_result;
}
