#pragma once
#include <string>
#include <memory>
#include "json.hpp"

namespace ORDO {

	class Request {

		struct RequestBlock {
			std::string data;
			long httpcode = 0;
		};
		std::string loicense;

		size_t callBack(void* contents, size_t size, size_t nmemb, void* userp);
		bool requestProtocol(const std::string& url, RequestBlock& endpoint, long& httpcode, std::string* response = nullptr);

	public:
		Request() = default;
		Request(const std::string& filePath, std::string* response = nullptr) {
			setLoicence(filePath, response);
		}
		bool setLoicence(const std::string& filePath, std::string* response = nullptr);
		std::unique_ptr<nlohmann::json> request(const std::string& url, std::string* resonse = nullptr);
	};

}