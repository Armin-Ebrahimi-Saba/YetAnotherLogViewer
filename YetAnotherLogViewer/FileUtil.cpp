#include "FileUtil.hpp"

using namespace YALV;

YALV::FileUtil::FileUtil(): parser()
{
}

void FileUtil::parseFile( const string& filename,
                          QList<QStringList>& logs) const
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Couldn't open a log file");
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        QStringList logLine = parser.parseLine(line);
        if (!logLine.empty())
            logs << logLine;
    }
}

void YALV::FileUtil::setRegex(const string& newRegex)
{
    parser.setRegex(newRegex);
}
