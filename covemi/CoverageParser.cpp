#include <fstream>

#include "CoverageParser.h"

CoverageParser::CoverageParser(const std::regex &executed, const std::regex &unexecuted, bool isCountBeforeLineNum)
    : executed(executed), unexecuted(unexecuted), isCountBeforeLineNum(isCountBeforeLineNum) {}

std::set<int> *CoverageParser::parse(std::string file)
{
    std::set<int> *unexecutedSet = new std::set<int>;
    std::ifstream ifs;
    ifs.open(file);
    if (!ifs.is_open())
    {
        throw std::runtime_error("can not open file");
        return unexecutedSet;
    }
    std::string line;
    while (std::getline(ifs, line))
    {
        std::smatch sm;
        if (std::regex_match(line, sm, unexecuted))
        {
            unexecutedSet->insert(std::stoi(sm[1]));
        }
        else if (std::regex_match(line, sm, executed))
        {
            int count;
            int lineNum;
            if (isCountBeforeLineNum)
            {
                count = std::stoi(sm[1]);
                lineNum = std::stoi(sm[2]);
            }
            else
            {
                count = std::stoi(sm[2]);
                lineNum = std::stoi(sm[1]);
            }
            executedMap.insert({lineNum, count});
        }
    }
    ifs.close();
    return unexecutedSet;
}

std::map<int, int> &CoverageParser::getExecutedMap()
{
    return executedMap;
}