#pragma once
#include <set>
#include <regex>
#include <string>

// Read code coverage report file of gcov and llvm-cov.
// The coverage report file name (which is <report-name>) format is 
// <source-name>.<extension>, Where <source-name> is the file name of 
// the source code and the <extension> of gcov and llvm-cov are 
// gcov and llvm-cov respectively
class CoverageParser
{
private:
    const std::regex regex;

public:
    CoverageParser(std::string regex) : regex(std::regex(regex)){};
    CoverageParser(std::regex regex) : regex(regex){};
    std::set<int> *parse(std::string file);
};
