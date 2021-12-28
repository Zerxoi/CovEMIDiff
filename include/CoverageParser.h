#pragma once
#include <set>
#include <regex>
#include <string>

class CoverageParser
{
private:
    const std::regex regex;

public:
    CoverageParser(std::string regex) : regex(std::regex(regex)){};
    CoverageParser(std::regex regex) : regex(regex){};
    std::set<int> *parse(std::string file);
};
