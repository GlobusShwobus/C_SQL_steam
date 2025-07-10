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

	class SteamGame: public SQLTable {
	public:
		std::string gameName;
		std::string lastPlayedDate;
		int gameVersion = 0;//not sure currently if i can extract this, it's located in the achievement metadata, which is kind of fucky
		int gameID = 0;
		int playTimeForever = 0;

		std::string_view tableName()const override {
			static constexpr std::string_view name = "games";
			return name;
		}

		std::string_view createTableStatement()const override {
			static constexpr std::string_view createTable =
				"CREATE TABLE games (game_id BIGINT PRIMARY KEY, name VARCHAR(75), game_version INT, total_playtime INT, last_played_date DATE); ";
			return createTable;
		}

		std::string_view createInsertStatement()const override {
			static constexpr std::string_view insertable =
				"INSERT INTO games (game_id, name, game_version, total_playtime, last_played_date) VALUES (?,?,?,?,?);";
			return insertable;
		}

		void bindToStatement(sql::PreparedStatement* stmt)const override {
			stmt->setUInt64(1, gameID);
			stmt->setString(2, gameName);
			stmt->setInt(3, gameVersion);
			stmt->setInt(4, playTimeForever);
			stmt->setDateTime(5, lastPlayedDate);
		}
	};

	class SteamActivity :public SQLTable {
	public:
		std::string gameName;
		int gameID = 0;
		int playtimeLastTwoWeeks = 0;
		int playtimeForever = 0;

		std::string_view tableName()const override {
			static constexpr std::string_view name = "activity";
			return name;
		}

		std::string_view createTableStatement()const override {
			static constexpr std::string_view createTable =
				"CREATE TABLE activity (game_id BIGINT PRIMARY KEY, name VARCHAR(75), total_playtime INT, playtime_fortnite INT); ";
			return createTable;
		}

		std::string_view createInsertStatement()const override {
			static constexpr std::string_view insertable =
				"INSERT INTO activity (game_id, name, total_playtime, playtime_fortnite) VALUES (?,?,?,?);";
			return insertable;
		}

		void bindToStatement(sql::PreparedStatement* stmt)const override {
			stmt->setUInt64(1, gameID);
			stmt->setString(2, gameName);
			stmt->setInt(3, playtimeForever);
			stmt->setInt(4, playtimeLastTwoWeeks);
		}
	};


	class sqlAchievementTable : public SQLTable {
	public:
		std::string achievementID;
		std::string achievementName;
		std::string description;
		std::string unlockDate;
		int gameID = 0;
		double globalCompRate = 0;
		bool isAchieved = 0;

		std::string_view tableName()const override {
			static constexpr std::string_view name = "achievements";
			return name;
		}

		std::string_view createTableStatement()const override {
			static constexpr std::string_view createTable =
				"CREATE TABLE achievements (game_id BIGINT, achievement_id VARCHAR(40), name VARCHAR(100), description VARCHAR(256), global_completion DECIMAL(5,2), is_achieved INTEGER, unlock_date DATE, PRIMARY KEY(game_id, achievement_id)); ";
			return createTable;
		}

		std::string_view createInsertStatement()const override {
			static constexpr std::string_view insertable =
				"INSERT INTO achievements (game_id, achievement_id, name, description, global_completion, is_achieved, unlock_date ) VALUES (?,?,?,?,?,?,?);";
			return insertable;
		}

		void bindToStatement(sql::PreparedStatement* stmt)const override {
			stmt->setUInt64(1, gameID);
			stmt->setString(2, achievementID);
			stmt->setString(3, achievementName);
			stmt->setString(4, description);
			stmt->setDouble(5, globalCompRate);
			stmt->setBoolean(6, isAchieved);
			stmt->setDateTime(7, unlockDate);
		}

	};
}