#include "Logger.hpp"


using namespace YALV;

std::mutex Logger::mut;
Logger* Logger::logger;


Logger::Logger(): fileUtil(), networkUtil() {}

void Logger::parseFile(const string& filePath, QList<QStringList>& logs) const
{
	fileUtil.parseFile(filePath, logs);
}

void Logger::saveLogsToFile(const QList<QStringList>& parsedLogs, const std::string& fileName) const
{
	QFile file{ QString::fromUtf8(fileName.c_str()) };
	if (!file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text))
		printf("could't open file %s: \nfile: %s\nline: %d", fileName.c_str(), __FILE__, __LINE__);
	QTextStream out(&file);
	char* tmp = new char(64);
	for (const QStringList& log : parsedLogs) {
		for (const QString& field : log)
			out << field << " ";
		out << "\n";
	}
}

void Logger::saveLogRecordsToDB(QList<QStringList>& parsedLogs) const
{
}

void YALV::Logger::fetchLogsTCP(QList<QStringList>& logs) const
{
	networkUtil.fetchLogsTCP(logs);
}

void YALV::Logger::fetchLogsUDP(QList<QStringList>& logs) const
{
	networkUtil.fetchLogsUDP(logs);
}

void Logger::setRegex(const string& newRegex)
{
	networkUtil.setRegex(newRegex);
	fileUtil.setRegex(newRegex);
}


void YALV::Logger::setEndpoint(const string& ipv4, const unsigned short& portNum)
{
	networkUtil.setEndpoint(ipv4, portNum);
}

Logger* Logger::getInstance()
{
#if DEBUG
	if (logger == nullptr)
		qDebug() << "Logger is not initialized.\nfile: " << __FILE__ << "\nline: " << __LINE__;
#endif 
	return logger;
}

void Logger::init()
{
#if DEBUG
	if (logger != nullptr)
	qDebug() << "Instance of logger must be first deleted before creating a new one.\nfile: " << __FILE__ << "\nline: " << __LINE__;
#endif 
	qDebug() << "creating new logger objet\n";
	std::lock_guard<std::mutex> lock(mut);
	logger = new Logger();
}

void Logger::destroyInstance()
{
#if DEBUG
	if (logger != nullptr)
		qDebug() << "Deleting null pointer to logger instance.\nfile: " << __FILE__ << "\nline: " << __LINE__;
#endif 
	if (logger != nullptr) {
		std::lock_guard<std::mutex> lock(mut);
		delete logger;
	}
}



/*
list<list<string>> Logger::parseFile(const string& filePath) const
{
	return parser->parse(filePath);
}
*/
