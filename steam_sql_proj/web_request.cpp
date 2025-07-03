#include "web_request.h"
#include <fstream>
#include <curl/curl.h>

namespace ORDO {
	size_t Request::callBack(void* contents, size_t size, size_t nmemb, void* userp) {
		size_t real_size = size * nmemb;
		RequestBlock* block = static_cast<RequestBlock*>(userp);
		block->data.append(static_cast<char*>(contents), real_size);
		return real_size;
	}
	bool Request::requestProtocol(const std::string& url, RequestBlock& endpoint, long& httpcode, std::string* response = nullptr) {
		CURL* curl;
		CURLcode result;

		curl = curl_easy_init();

		if (curl == NULL) {
			if (response)
				*response = "libcurl failed to initalize";
			return false;
		}
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());//fuck my ass, insert url as string -> get wider asshole (good i commented this)

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Request::callBack);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &endpoint);

		curl_easy_setopt(curl, CURLOPT_CAINFO, loicense.c_str());

		result = curl_easy_perform(curl);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, httpcode);

		if (result != CURLE_OK) {
			curl_easy_cleanup(curl);
			if (response)
				*response = "Libcurl err: " + std::string(curl_easy_strerror(result));
			return false;
		}

		curl_easy_cleanup(curl);

		return true;
	}
	bool Request::setLoicence(const std::string& filePath, std::string* response = nullptr) {
		bool result = false;

		std::ifstream file(filePath);

		if (file.good()) {
			loicense = filePath;
			result = true;

			if (response) {
				*response = "file path established with >> " + filePath;
			}

		}
		else {
			loicense.clear();
			if (response) {
				*response = "UNABLE TO DETERMINE PATH TO CERTIFICATE";
			}
		}
		file.close();

		return result;
	}

	std::unique_ptr<nlohmann::json> Request::request(const std::string& url, std::string* resonse = nullptr)
	{
		if (loicense.empty()) {
			if (resonse)
				*resonse = "Path to certificate cacert.pem not established";
			return nullptr;
		}
		std::unique_ptr<RequestBlock> endpoint = std::make_unique<RequestBlock>();
		long httpcode = 0;

		if (!requestProtocol(url, *endpoint, httpcode, resonse)) {

			if (resonse)
				//func() http code error message here

				return nullptr;
		}
		return std::make_unique<nlohmann::json>(nlohmann::json::parse(endpoint.get()->data));
	}
}