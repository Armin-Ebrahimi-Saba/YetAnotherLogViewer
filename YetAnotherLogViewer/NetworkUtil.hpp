#pragma once

#ifndef YALV_NETWORKUTIL_HPP
#define YALV_NETWORKUTIL_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <ctime>
#include <chrono>

#include <QStringList>
#include <QString>
#include <QDebug>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/array.hpp>
#include <boost/tokenizer.hpp>

#include "blockingconcurrentqueue.h"
#include "Parser.hpp"

using std::string;
using cQueue = moodycamel::BlockingConcurrentQueue<std::string>;
using namespace boost;


namespace YALV {
    class NetworkUtil {
    public:
        NetworkUtil();
        ~NetworkUtil() = default;
        void readTCPSock(  asio::ip::tcp::socket& sock,
                        cQueue& logLines,
                        QList<QStringList>& logs) const;
        void readUDPSock(  asio::ip::udp::socket& sock,
                        cQueue& logLines,
                        QList<QStringList>& logs) const;
        void parseLogLines( cQueue& logLines,
                            QList<QStringList>& logs) const;
        int fetchLogsTCP(QList<QStringList>& logs) const;
        int fetchLogsUDP(QList<QStringList>& logs) const;
        void setRegex(const string& newRegex);
        void setEndpoint(const string& newIpv4,
                         const unsigned short& newPortNum);
    private:
        Parser parser;
        string rawIpv4Address = "127.0.0.1";
        unsigned short portNum = 3334;
    };
}

#endif 