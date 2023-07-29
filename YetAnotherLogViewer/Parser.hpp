#pragma once

#ifndef YALV_PARSER_HPP
#define YALV_PARSER_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <list>
#include <QList>
#include <QStringList>
#include <QString>

using std::list;
using std::string;

namespace YALV {
    class Parser
    {
    public:
        Parser() = default;
        ~Parser() = default;
        void setRegex(const string& rawRegex);
        QStringList parseLine(const string& line) const;

    private:
        string rawRegex = R"(\[(\d{4}\-\d{2}\-\d{2} \d{2}:\d{2}:\d{2}\.\d{3})\] \[(0x[\dabcdef]{1,})\] \[(DEBUG|ERROR|WARNING|INFO)\] (.*))";
    };
}
#endif // YALV_PARSER_HPP

//QList<QStringList> parseStringList(const QStringList& logs) const;
//list<list<string>> parse(const string& filename);
//list<string> parseLine(const string& line);