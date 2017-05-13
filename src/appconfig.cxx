#include "appconfig.hxx"

AppConfig::AppConfig(const std::string& config_file_path) {
	FILE *fp = fopen(config_file_path.c_str(), "r");
	char read_buf[65536];
	rapidjson::FileReadStream is(fp, read_buf, sizeof(read_buf));

	this->config_doc.ParseStream(is);
	fclose(fp);

	this->valid_config = this->config_doc.IsObject();
}

std::string AppConfig::getString(const std::string& value) {
	if (!this->valid_config || !this->config_doc.HasMember(value.c_str())) {
		return "";
	}

	const rapidjson::Value& conf_val = this->config_doc[value.c_str()];

	if (!conf_val.IsString()) {
		return "";
	}

	return conf_val.GetString();
}
