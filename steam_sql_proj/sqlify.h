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
		std::string accountUrl;
		std::string logoffDate;
		std::string creationDate;
		std::string countryCode;
	};

	struct sqlGamesTable {
		int gameId = 0;
		int gameVersion = 0;//not sure currently if i can extract this, it's located in the achievement metadata, which is kind of fucky
		std::string gameName;
		int playTimeForever = 0;
		std::string lastPlayedData;
	};

	struct sqlActivityTable {
		int gameId = 0;
		std::string gameName;
		int playtimeLastTwoWeeks = 0;
		int playtimeForever = 0;
	};

	struct sqlAchievemntTable {
		std::string gameName;
		std::string achievementName;
		std::string description;
		bool isAchieved;
		std::string unlockDate;
		float globalCompRate;
	};
}