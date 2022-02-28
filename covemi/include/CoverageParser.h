#pragma once
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

// Read code coverage report file of gcov and llvm-cov.
// The coverage report file name (which is <report-name>) format is
// <source-name>.<extension>, Where <source-name> is the file name of
// the source code and the <extension> of gcov and llvm-cov are
// gcov and llvm-cov respectively
class CoverageParser {
 public:
  CoverageParser(const std::regex &executed, const std::regex &unexecuted, bool isCountBeforeLineNum);
  std::set<int> *parse(std::string file);
  std::map<int, int> &getCountMap();

 private:
  const std::regex executed;
  const std::regex unexecuted;
  const bool isCountBeforeLineNum;
  std::map<int, int> countMap;
};
