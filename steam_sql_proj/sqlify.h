#pragma once

#include <string>
#include <string_view>
#include <memory>

#include <cppconn/prepared_statement.h>//for prep statements

#include <mysql_driver.h>    //for driver
#include <mysql_connection.h>//for connection
#include <cppconn/driver.h>  //for connecting driver with connector

#include "NumberGenerator.h"//cpp


namespace ORDO {


	class SSQLUser {

		std::string serverIP;
		std::string serverName;
		std::string serverPassword;

	public:

		SSQLUser() = default;
		SSQLUser(std::string_view IP, std::string_view server_name, std::string_view password)
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

	class SSQLTable {
	public:

		virtual std::string_view createTableStatement() const = 0;
		virtual std::string_view createInsertStatement()const = 0;
		virtual void bindToStatement(sql::PreparedStatement* stmt)const = 0;
		virtual std::string_view tableName()const = 0;
		virtual ~SSQLTable() = default;
	};

	class SSQLFlight {

		sql::mysql::MySQL_Driver* mDriver = nullptr;
		std::unique_ptr<sql::Connection> mConnect = nullptr;
		bool mIsGood = false;
		std::string SSQLStreamError;

		static constexpr std::string_view defaultLocalServer = "tcp://127.0.0.1:3306";

		bool Connect(std::string_view IP, std::string_view user, std::string_view password)
		{
			//if for any reason attempt a new connection mid flight, clean up old shit
			mConnect.release();
			mDriver = nullptr;
			mIsGood = false;
			SSQLStreamError.clear();

			//establish driver, can fail i guess but should not be likely
			try {
				mDriver = sql::mysql::get_driver_instance();
			}
			catch (const sql::SQLException& expt) {
				SSQLStreamError = "Driver init fail: " + std::string(expt.what());
				return false;
			}

			//establish connection for the flight, can fail for all reasons
			try {
				mConnect = std::unique_ptr<sql::Connection>(mDriver->connect(IP.data(), user.data(), password.data()));
				mIsGood = true;
			}
			catch (const sql::SQLException& expt) {
				SSQLStreamError = "Connection init fail: " + std::string(expt.what());
				mDriver = nullptr;
				return false;
			}

			return true;
		}

	public:
		SSQLFlight() = default;
		SSQLFlight(std::string_view IP, std::string_view user, std::string_view password)
		{
			Connect(IP, user, password);
		}
		SSQLFlight(std::string_view user, std::string_view password)
		{
			Connect(defaultLocalServer, user, password);
		}

		bool connect(std::string_view IP, std::string_view user, std::string_view password) {
			return Connect(IP, user, password);
		}
		bool connect(std::string_view user, std::string_view password) {
			return Connect(defaultLocalServer, user, password);
		}


		bool isGood()const {
			return mIsGood;
		}
		std::string_view getStreamError()const {
			return SSQLStreamError;
		}

	public:
		//don't want these for streams
		SSQLFlight(const SSQLFlight&) = delete;
		SSQLFlight(SSQLFlight&&) = delete;
		SSQLFlight& operator=(const SSQLFlight&) = delete;
		SSQLFlight& operator=(SSQLFlight&&) = delete;

		// SIMILAR TO FILESTREAMS
		// 
		//pointers to connection
		//pointer to statement
		//other

		//functions if good, inject, close
		//destructor does automatic close as well
	};

}
