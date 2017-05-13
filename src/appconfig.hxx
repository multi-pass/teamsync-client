#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <fstream>
#include <string>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include "rapidjson/filereadstream.h"

#include <stdio.h>

class AppConfig {
	public:
	AppConfig(const std::string& config_file_path);

	std::string getString(const std::string& value);

	private:
	bool valid_config;
	rapidjson::Document config_doc;
};

#endif
