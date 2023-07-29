#include "Parser.hpp"

using namespace YALV;
using std::list;
using std::string;


void Parser::setRegex(const string& newRegex)
{
    rawRegex = newRegex;
}


QStringList Parser::parseLine(const std::string& oneLine) const
{
    std::regex reg(rawRegex);
    std::smatch match;
    if (std::regex_search(oneLine, match, reg))
    {
        QStringList logLine;
        for (size_t i = 1; i < match.size(); i++)
            logLine << match.str(i).c_str();
        return logLine;
    }
    return {};
}

/*
QList<QStringList> Parser::parseStringList(const QStringList& lines) const
{
    QList<QStringList> logs;
    for (auto log : lines) {
        if (log.isEmpty())
            continue;
        QStringList logLine = parseLine(log.toStdString());
        if (!logLine.empty())
            logs << logLine;
    }
    return logs;

list<list<string>> Parser::parse(const string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Couldn't open a log file");
    list<list<string>> logs;
    string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        QStringList logLine = parseLine(line);
        if (!logLine.empty())
            logs.push_back(logLine);
    }
    return logs;
}

list<string> Parser::parseLine(const string& oneLine)
{
    std::regex reg(rawRegex);
    std::smatch match;
    if (std::regex_search(oneLine, match, reg))
    {
        list<string> logLine(5);
        for (size_t i = 1; i < match.size(); i++)
            logLine.push_back(match.str(i).c_str());
        return logLine;
    }
    return {};
}
*/

