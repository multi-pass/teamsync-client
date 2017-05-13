#include "servercommunicator.hxx"

void traverseTree(const std::string& path, const rapidjson::Value& tree_obj,
				  void (*callback)(const std::string&, const std::string&));

ServerCommunicator::ServerCommunicator(const std::string& server_url)
	: server_url(server_url) {

}

bool ServerCommunicator::authenticate(const std::string& pgpid) {

}

void ServerCommunicator::getFullTree(void (*callback)(const std::string&,
													  const std::string&)) {
	APIRequest req(GET, this->server_url, "/secrets");
	APIResponse resp = req.execute();

	rapidjson::Document json_document;
	json_document.Parse(resp.http_response.c_str());

	if (json_document.IsObject()) {
		traverseTree("", json_document, callback);
	}
void traverseTree(const std::string& path, const rapidjson::Value& tree_obj,
				  void (*callback)(const std::string&, const std::string&)) {
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
			(*callback)(secret_path, hash_str);
		}
	}

	if (tree_obj.HasMember("folders") && tree_obj["folders"].IsArray()) {
		const rapidjson::Value& folders = tree_obj["folders"];
		for (rapidjson::Value::ConstValueIterator it = folders.Begin(),
				 end = folders.End(); it != end; ++it) {
			// Process subfolders recursively
			traverseTree(cur_path, *it, callback);
		}
	}
}

int ServerCommunicator::addSecret() {

}

int ServerCommunicator::setSecret() {

}

int ServerCommunicator::deleteSecret() {

}



/// API classes

APIRequest::APIRequest(HTTPMethod method, std::string server_url,
						 std::string api_route) {
	if (!(this->curl = curl_easy_init())) {
		// TODO: Throw exception
	}

	// Default options
	curl_easy_setopt(this->curl, CURLOPT_NOPROGRESS, 1L);

	switch (method) {
	case GET:
		curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1L);
		break;
	case POST:
		curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1L);
		break;
	default:
		// Unsupported method
		// TODO: Throw exception
		break;
	}

	if (api_route[0] != '/') {
		// Invalid route (must begin with a slash)
		// TODO: Throw exception
	}

	std::string request_uri = (server_url + api_route);

	curl_easy_setopt(this->curl, CURLOPT_URL, request_uri.c_str());
}

APIResponse APIRequest::execute() {
	APIResponse response;

	size_t (*write_callback)(char *, size_t, size_t, void *);
	write_callback = &(APIResponse::writeCallback);

	curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, write_callback);

	curl_easy_perform(this->curl);

	return response;
}

APIResponse::APIResponse() {

}

size_t APIResponse::receiveData(char *ptr, size_t size, size_t nmemb) {
	size_t total_length = (nmemb * size);

	// Store response
	this->http_response = std::string(ptr, total_length);

	return total_length;
}

size_t APIResponse::writeCallback(char *ptr, size_t size, size_t nmemb,
								  void *response_obj) {
	return ((APIResponse *)response_obj)->receiveData(ptr, size, nmemb);
}
