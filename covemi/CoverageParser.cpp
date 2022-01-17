#include <fstream>

#include "CoverageParser.h"

std::set<int> *CoverageParser::parse(std::string file)
{
    std::set<int> *unexecuted = new std::set<int>;
    std::ifstream ifs;
    ifs.open(file);
    if (!ifs.is_open())
    {
        throw std::runtime_error("can not open file");
        return unexecuted;
    }
    std::string line;
    while (std::getline(ifs, line))
    {
        std::smatch sm;
        if (std::regex_search(line, sm, regex))
        {
            unexecuted->insert(std::stoi(sm[1]));
        }
    }
    ifs.close();
    return unexecuted;
}
