#include <fstream>
#include "CovParser.h"

CovParser::CovParser(std::string separator, int countIndex, int numIndex) : separator(separator), countIndex(countIndex), numIndex(numIndex)
{
    re = std::regex("^\\s*(\\d+)" + separator + "\\s*(\\d+)" + separator + "(.*)$");
}

std::map<int, int> *CovParser::parse(std::string file)
{
    std::map<int, int> *covMap = new std::map<int, int>;
    std::ifstream ifs;
    ifs.open(file);
    if (!ifs.is_open())
    {
        throw std::runtime_error("can not open file");
    }
    std::string line;
    while (std::getline(ifs, line))
    {
        std::smatch sm;
        if (std::regex_search(line, sm, re))
        {
            int count = std::stoi(sm[countIndex]);
            int num = std::stoi(sm[numIndex]);
            std::string content = sm[3].str();
            if (!covMap->count(num))
            {
                (*covMap)[num] = count;
            }
        }
    }
    ifs.close();
    return covMap;
}
