#include "web_request.h"
#include <fstream>
#include <curl/curl.h>

namespace ORDO {
	size_t Request::callBack(void* contents, size_t size, size_t nmemb, void* userp) {
		size_t realSize = size * nmemb;
		ResoponseBuffer* buffer = static_cast<ResoponseBuffer*>(userp);
		char* begin = static_cast<char*>(contents);
		buffer->data.insert(buffer->data.end(), begin, begin + realSize);
		return realSize;
	}
	bool Request::requestProtocol(const std::string& url, ResoponseBuffer& endpoint, std::string* response) {
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

		if (result != CURLE_OK) {
			curl_easy_cleanup(curl);
			if (response)
				*response = "Libcurl err: " + std::string(curl_easy_strerror(result));
			return false;
		}
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, endpoint.httpcode);
		char* content_type = nullptr;
		curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
		if (content_type)
			endpoint.contentType = content_type;

		curl_easy_cleanup(curl);
		endpoint.success = true;
		if (response)
			*response = "success";
		return true;
	}
	bool Request::setLoicence(const std::string& filePath, std::string* response) {
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

	std::unique_ptr<Request::ResoponseBuffer> Request::request(const std::string& url, std::string* resonse)
	{
		if (loicense.empty()) {
			if (resonse)
				*resonse = "Path to certificate cacert.pem not established";
			return nullptr;
		}
		std::unique_ptr<ResoponseBuffer> endpoint = std::make_unique<ResoponseBuffer>();

		if (!requestProtocol(url, *endpoint, resonse))
			endpoint.get()->data.clear();

		//func() http code error message not here actually
		return std::move(endpoint);
	}
}