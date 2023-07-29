#pragma once

#ifndef YALV_FILEUTIL_HPP
#define YALV_FILEUTIL_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <ctime>

#include "blockingconcurrentqueue.h"
#include "Parser.hpp"

using std::string;
using cQueue = moodycamel::BlockingConcurrentQueue<std::string>;


namespace YALV {
    class FileUtil {
    public:
        FileUtil();
        ~FileUtil() = default;
        void parseFile( const string& filename,
                        QList<QStringList>& logs) const;
        void setRegex(const string& newRegex);
    private:
        Parser parser;
    };
}


#endif 