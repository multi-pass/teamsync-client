#include "servercommunicator.hxx"

void traverseTree(const std::string& path, const rapidjson::Value& tree_obj,
				  void (*callback)(const std::string&, const std::string&,
								   void *), void *);

ServerCommunicator::ServerCommunicator(const std::string& server_url)
	: server_url(server_url) {

}

bool ServerCommunicator::authenticate(const std::string& pgpid) {
	APIRequest req(POST, this->server_url, "/auth/requestlogin", this->session);

	rapidjson::Document req_data;
	req_data.SetObject();
	req_data.AddMember("pgpid", pgpid, req_data.GetAllocator());

	rapidjson::StringBuffer json_buf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(json_buf);
	req_data.Accept(writer);
	req.setPOSTFields(json_buf.GetString());
	APIResponse resp = req.send();
}

void ServerCommunicator::getFullTree(void (*callback)(const std::string&,
													  const std::string&,
													  void *), void *userdata) {
	APIRequest req(GET, this->server_url, "/secrets", this->session);
	APIResponse resp = req.send();

	rapidjson::Document json_document;
	json_document.Parse(resp.http_response.c_str());

	if (json_document.IsObject()) {
		traverseTree("", json_document, callback, userdata);
	}
void traverseTree(const std::string& path, const rapidjson::Value& tree_obj,
				  void (*callback)(const std::string&, const std::string&,
								   void *), void *callback_userdata) {
	std::string cur_path;

	std::string folder_comp(tree_obj["path"].GetString());
	if (folder_comp != "/") {
		cur_path = (path + "/" + folder_comp);
	}

	if (tree_obj.HasMember("secrets") && tree_obj["secrets"].IsArray()) {
		const rapidjson::Value& secrets = tree_obj["secrets"];
		for (rapidjson::Value::ConstValueIterator it = secrets.Begin(),
				 end = secrets.End(); it != end; ++it) {
			const rapidjson::Value& secret(*it);
			const std::string secret_name(secret["name"].GetString());
			const std::string secret_path((cur_path + "/" + secret_name));
			std::string hash_str("");
			const rapidjson::Value& hash_obj = secret["hash"];
			if (hash_obj.IsObject()) {
				if (hash_obj.HasMember("sha256")) {
					hash_str = (std::string("sha256:")
								+ hash_obj["sha256"].GetString());
				} else if (hash_obj.HasMember("ripemd160")) {
					hash_str = (std::string("ripemd160:")
								+ hash_obj["ripemd160"].GetString());
				} else if (hash_obj.HasMember("sha1")) {
					hash_str = (std::string("sha1:")
								+ hash_obj["sha1"].GetString());
				} else {
					fprintf(stderr, "Unsupported hash algorithms for secret %s.\n",
							secret_path.c_str());
				}
			}

			// Call callback with secret
			(*callback)(secret_path, hash_str, callback_userdata);
		}
	}

	if (tree_obj.HasMember("folders") && tree_obj["folders"].IsArray()) {
		const rapidjson::Value& folders = tree_obj["folders"];
		for (rapidjson::Value::ConstValueIterator it = folders.Begin(),
				 end = folders.End(); it != end; ++it) {
			// Process subfolders recursively
			traverseTree(cur_path, *it, callback, callback_userdata);
		}
	}
}

const std::string /* base64 */ ServerCommunicator::getSecret(const std::string& path) {
	APIRequest req(GET, this->server_url, ("/secrets" + path), this->session);
	APIResponse resp = req.send();

	rapidjson::Document json_document;
	json_document.Parse(resp.http_response.c_str());

	if (!json_document.IsObject()) {
		fprintf(stderr, "Getting secret %s failed.\n", path.c_str());
		return "";
	}

	if (!json_document.HasMember("payload")) {
		fprintf(stderr, "Secret %s is invalid.\n", path.c_str());
		return "";
	}

	return json_document["payload"].GetString();
}

int ServerCommunicator::setSecret() {

}

int ServerCommunicator::deleteSecret() {

}
