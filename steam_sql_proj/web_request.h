#pragma once
#include <string>
#include <vector>
#include <memory>

namespace ORDO {

	class Request {

		struct ResoponseBuffer {
			std::vector<char> data;
			long httpcode = 0;
			bool success = false;
			std::string contentType;
		};
		std::string loicense;

		static size_t callBack(void* contents, size_t size, size_t nmemb, void* userp);
		bool requestProtocol(const std::string& url, ResoponseBuffer& endpoint, std::string* response = nullptr);

	public:
		Request() = default;
		Request(const std::string& filePath, std::string* response = nullptr) {
			setLoicence(filePath, response);
		}
		bool setLoicence(const std::string& filePath, std::string* response = nullptr);
		std::unique_ptr<ResoponseBuffer> request(const std::string& url, std::string* resonse = nullptr);
	};

}