#pragma once

#include <string>
#include <string_view>

namespace ORDO {


	class sqlInjector {

		std::string serverIP;
		std::string serverName;
		std::string serverPassword;

	public:

		sqlInjector() = default;
		sqlInjector(std::string_view IP, std::string_view server_name, std::string_view password)
			:serverIP(IP), serverName(server_name), serverPassword(password) {}

		void setServerIP(std::string_view ip) {
			serverIP = ip;
		}
		void setServerName(std::string_view name) {
			serverName = name;
		}
		void setServerPassword(std::string_view password) {
			serverPassword = password;
		}
		std::string_view getIP()const {
			return serverIP;
		}
		std::string_view getSName()const {
			return serverName;
		}
		std::string_view getSPassword()const {
			return serverPassword;
		}

	};

	struct sqlSummaryTable {
		uint64_t playerId = 0;
		std::string name;
		std::string logoffDate;
		std::string creationDate;
		std::string countryCode;
	};

	struct sqlGamesTable {
		int gameCodeId = 0;
		std::string gameName;
		int playTimeMinutes = 0;
		std::string lastPlayedData;
	};

	struct sqlActivityTable {
		int gameId = 0;
		std::string gameName;
		int playtimeLastTwoWeeks = 0;
		int playtimeForever = 0;
	};

}