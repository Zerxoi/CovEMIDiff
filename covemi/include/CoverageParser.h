#pragma once
#include <map>
#include <set>
#include <regex>
#include <string>
#include <vector>

// Read code coverage report file of gcov and llvm-cov.
// The coverage report file name (which is <report-name>) format is
// <source-name>.<extension>, Where <source-name> is the file name of
// the source code and the <extension> of gcov and llvm-cov are
// gcov and llvm-cov respectively
class CoverageParser
{
private:
    const std::regex executed;
    const std::regex unexecuted;
    const bool isCountBeforeLineNum;
    static std::map<int, int> executedMap;
    static std::vector<int> diffLines;

public:
    CoverageParser(const std::regex &executed, const std::regex &unexecuted, bool isCountBeforeLineNum);
    std::set<int> *parse(std::string file);
    static std::vector<int> &getDiffLines();
};
