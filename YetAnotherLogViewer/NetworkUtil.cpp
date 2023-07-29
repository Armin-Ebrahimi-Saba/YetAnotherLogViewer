#include "NetworkUtil.hpp"
#include "blockingconcurrentqueue.h"

#define DEBUG 1

using namespace YALV;
using cQueue = moodycamel::BlockingConcurrentQueue<std::string>;

YALV::NetworkUtil::NetworkUtil(): parser()
{
}

void NetworkUtil::readTCPSock(	asio::ip::tcp::socket& sock,
								cQueue& logLines,
								QList<QStringList>& logs) const
{
	asio::streambuf buf;
	sock.set_option(asio::detail::socket_option::integer<SOL_SOCKET, SO_RCVTIMEO>{ 200 });
	try {
		while (size_t n = asio::read_until(sock, buf, "\n")) {
			logLines.enqueue(std::move(asio::buffer_cast<const char*>(buf.data())));
			buf.consume(n);
		}
	} catch (system::system_error& e) {
		qDebug() << "Error occured! Error code = " << e.code().value()
		<< ". Message: " << e.what();
	}
}

void NetworkUtil::readUDPSock(	asio::ip::udp::socket& sock,
								cQueue& logLines,
								QList<QStringList>& logs) const
{
	sock.set_option(asio::detail::socket_option::integer<SOL_SOCKET, SO_RCVTIMEO>{ 1000 });
	//boost::array<char, 512> recvBuf;
	std::string recvBuf;
	recvBuf.resize(128);
	asio::ip::udp::endpoint sendEp;
	boost::char_separator<char> sep{"\n"};
	try {
		while (size_t len = sock.receive(asio::buffer(recvBuf, 128))) {
			//qDebug() << "read something from the ocket";
			tokenizer tok{recvBuf, sep};
			for (const auto& line : tok) {
				//qDebug() << "new line of log: " << recvBuf;
				logLines.enqueue(line);
			}
		}
	}
	catch (system::system_error& e) {
		qDebug() << "Error occured! Error code = " << e.code().value()
			<< ". Message: " << e.what();
	}		
}

void NetworkUtil::parseLogLines(cQueue& logLines,
								QList<QStringList>& logs) const
{
	std::string buf;
	//while (true) {
		while ((logLines.wait_dequeue_timed(buf, std::chrono::milliseconds(1000)))) {
			if (buf.empty())
				continue;
			QStringList logLine = parser.parseLine(buf);
			if (!logLine.empty())
				logs << logLine;
		}
		//std::this_thread::sleep_for(std::chrono::seconds(22));
	//}
}

int NetworkUtil::fetchLogsTCP(QList<QStringList>& logs) const
{
	//string raw_ip_address = "127.0.0.1";
	//unsigned short port_num = 3333;
	cQueue logLines;
	boost::system::error_code ec;
	try {
		asio::ip::tcp::endpoint ep(
			asio::ip::address::from_string(rawIpv4Address),
			portNum);
		asio::io_service ios;
		asio::ip::tcp::socket sock(ios, ep.protocol());
		sock.connect(ep);
		qDebug() << "creating producer \n";
		std::thread producer(&NetworkUtil::readTCPSock, this, std::ref(sock), std::ref(logLines), std::ref(logs));
		qDebug() << "creating TCP consumer \n";
		std::thread consumer(&NetworkUtil::parseLogLines, this, std::ref(logLines), std::ref(logs));
		qDebug() << "waiting for producer(socket reader) \n";
		producer.join();
		qDebug() << "waiting for consumer(log line parser) \n";
		consumer.join();
	} catch (system::system_error& e) {
		qDebug() << "Error occured! Error code = " << e.code().value()
		<< ". Message: " << e.what();
		return e.code().value();
	}
	qDebug() << "finished fetching logs TCP\n";
	return 0;
}


int NetworkUtil::fetchLogsUDP(QList<QStringList>& logs) const {
	//string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3334;
	cQueue logLines;
	boost::system::error_code ec;
	try {
		asio::ip::udp::endpoint recvEp(
			asio::ip::address::from_string(rawIpv4Address),
			portNum);
		asio::io_service ios;
		asio::ip::udp::socket sock(ios, recvEp.protocol());
		boost::array<char, 1> sendBuf = { 0 };
		sock.send_to(asio::buffer(sendBuf), recvEp);
		qDebug() << "creating UDP producer \n";
		std::thread producer(&NetworkUtil::readUDPSock, this, std::ref(sock), std::ref(logLines), std::ref(logs));
		qDebug() << "creating consumer \n";
		std::thread consumer(&NetworkUtil::parseLogLines, this, std::ref(logLines), std::ref(logs));
		qDebug() << "waiting for producer(socket reader) \n";
		producer.join();
		qDebug() << "waiting for consumer(log line parser) \n";
		consumer.join();
	}
	catch (system::system_error& e) {
		qDebug() << "Error occured! Error code = " << e.code().value()
			<< ". Message: " << e.what();
		return e.code().value();
	}
	qDebug() << "finished fetching logs UDP\n";
	return 0;
}

void NetworkUtil::setRegex(const string& newRegex)
{
	parser.setRegex(newRegex);
}

void NetworkUtil::setEndpoint(const string& newIpv4,
	const unsigned short& newPortNum)
{
	rawIpv4Address = newIpv4;
	portNum = newPortNum;
#if DEBUG
	qDebug() << rawIpv4Address << "   " << portNum;
#endif
}
