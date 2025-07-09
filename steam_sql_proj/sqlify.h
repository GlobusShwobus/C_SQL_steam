#pragma once

#include <string>
#include <string_view>

#include <cppconn/prepared_statement.h>

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

	class SQLTable {
	public:

		virtual std::string_view createTableStatement() const = 0;
		virtual std::string_view createInsertStatement()const = 0;
		virtual void bindToStatement(sql::PreparedStatement* stmt)const = 0;
		virtual std::string_view tableName()const = 0;
		virtual ~SQLTable() = default;

	};

	class SteamSummary :public SQLTable {
	public:
		std::string name;
		std::string accountUrl;
		std::string countryCode;
		std::string creationDate;
		long long playerId = 0;
		int playerLevel = 0;
	
		std::string_view tableName()const override {
			static constexpr std::string_view name = "summary";
			return name;
		}

		std::string_view createTableStatement()const override {
			static constexpr std::string_view createTable =
				"CREATE TABLE summary (user_id BIGINT PRIMARY KEY, name VARCHAR(25), level INT, country_code VARCHAR(10), creation_date DATE, account_url VARCHAR(125)); ";
			return createTable;
		}

		std::string_view createInsertStatement()const override {
			static constexpr std::string_view insertable =
				"INSERT INTO summary (user_id, name, level, country_code, creation_date, account_url) VALUES (?,?,?,?,?,?);";
			return insertable;
		}

		void bindToStatement(sql::PreparedStatement* stmt)const override {
			stmt->setUInt64(1, playerId);
			stmt->setString(2, name);
			stmt->setInt(3, playerLevel);
			stmt->setString(4, countryCode);
			stmt->setDateTime(5, creationDate);
			stmt->setString(6, accountUrl);
		}
		
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