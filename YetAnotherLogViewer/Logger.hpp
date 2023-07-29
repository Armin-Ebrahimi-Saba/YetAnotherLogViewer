#pragma once

#ifndef YALV_LOGGER_HPP
#define YALV_LOGGER_HPP

#include <unordered_map>
#include <string>
#include <iostream>
#include <mutex>

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <blockingconcurrentqueue.h>

#include "NetworkUtil.hpp"
#include "FileUtil.hpp"

using std::string;
using std::list;
using cQueue = moodycamel::BlockingConcurrentQueue<string>;

namespace YALV {

	class Logger {
	public:
		Logger();
		~Logger() = default;
		bool operator==(const Logger& other) const = delete;

		/*
		* @brief takes a log file and output a list of parsed logs
		* @param filePath path of file to be parsed
		* @return list of parsed logs
		*/
		void parseFile(const string& filePath, QList<QStringList>& logs) const;


		/*
		* @brief takes a log message and write the parsed message with label to file
		* @param parsedLogs parsed logs to be written in a the file (fileName)
		* @param fileName name of the file to write log messages to
		*/
		void saveLogsToFile(const QList<QStringList>& parsedLogs,
							const string& fileName = "log.log") const;


		/*
		* @brief takes a log message and create a record of parsed message inside the DB
		* @param msg message (probably log message) to be parsed
		*/
		void saveLogRecordsToDB(QList<QStringList>& parsedLogs) const;
		
		/*
		* @brief fetch logs from server
		* @param logs list of logs to be filled
		*/
		void fetchLogsTCP(QList<QStringList>& logs) const;

		/*
		* @brief fetch logs from server
		* @param logs list of logs to be filled
		*/
		void fetchLogsUDP(QList<QStringList>& logs) const;


		/*
		* @brief set the regular expression for both NetworkUtil and FileUtil
		* @param newRegex is the new regular expression
		*/
		void setRegex(const string& newRegex);

		/*
		* @brief set the endpoint of the tcp connection
		* @param ipv4 of the server to connect to
		* @param portNum port number of server
		*/
		void setEndpoint(const string& ipv4, 
						const unsigned short& portNum);

		/*
		* @brief return the instance of the class
		* @return global logger (we are using singleton pattern)
		*/
		static Logger* getInstance();

		/*
		* @brief initializes the single instance of the class
		*/
		static void init();

		/*
		* @brief destroy the instance of the class
		*/
		static void destroyInstance();

	private:
		static Logger* logger;
		static std::mutex mut;
		FileUtil fileUtil;
		NetworkUtil networkUtil;
	};
}

#endif



/*
* @brief takes a log file and output a list of parsed logs
* @param filePath path of file to be parsed
* @return list of parsed logs
*/
//list<list<string>> parseFile(const string& filePath) const;
//Logger(const std::string&& delimeter, const std::string&& regex, const bool&& type);
//Logger(const QString& delimeter, const QString& regex, bool type) = delete;