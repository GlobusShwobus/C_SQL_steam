#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <memory>

namespace ORDO {

	class Request {
	public:

		struct ResoponseBuffer {
			std::vector<char> data;
			long httpcode = 0;
			bool isSuccess = false;
			std::string contentType;
		};

	private:

		std::string loicense;

		static size_t callBack(void* contents, size_t size, size_t nmemb, void* userp);
		bool requestProtocol(std::string_view url, ResoponseBuffer& reciever, std::string* extraInfo = nullptr);

	public:
		Request() = default;
		Request(std::string_view filePath, std::string* extraInfo = nullptr) {
			setLoicence(filePath, extraInfo);
		}
		bool setLoicence(std::string_view filePath, std::string* extraMessage = nullptr);
		std::unique_ptr<ResoponseBuffer> request(std::string_view url, std::string* extraInfo = nullptr);
	};

}