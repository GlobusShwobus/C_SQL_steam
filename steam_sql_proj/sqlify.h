#pragma once

#include <string>
#include <string_view>

namespace ORDO {


	class sqlUser {

		std::string serverIP;
		std::string serverName;
		std::string serverPassword;

	public:

		sqlUser() = default;
		sqlUser(std::string_view IP, std::string_view server_name, std::string_view password)
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

	class sqlConnection {
		// SIMILAR TO FILESTREAMS
		// 
		//pointers to connection
		//pointer to statement
		//other

		//functions if good, inject, close
		//destructor does automatic close as well
	};


	struct sqlSummaryTable {
		std::string name;
		std::string accountUrl;
		std::string logoffDate;
		std::string creationDate;
		std::string countryCode;
		uint64_t playerId = 0;
		int playerLevel;
	};

	struct sqlGamesTable {
		std::string gameName;
		std::string lastPlayedData;
		int gameId = 0;
		int gameVersion = 0;//not sure currently if i can extract this, it's located in the achievement metadata, which is kind of fucky
		int playTimeForever = 0;
	};

	struct sqlActivityTable {
		std::string gameName;
		int gameId = 0;
		int playtimeLastTwoWeeks = 0;
		int playtimeForever = 0;
	};

	struct sqlAchievemntTable {
		std::string gameName;
		std::string achievementID;
		std::string achievementName;
		std::string description;
		std::string unlockDate;
		float globalCompRate;
		bool isAchieved;
	};
}