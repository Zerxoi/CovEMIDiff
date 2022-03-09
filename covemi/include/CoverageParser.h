#pragma once
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

// Read code coverage report file of gcov and llvm-cov.
class CoverageParser {
 public:
  CoverageParser(const std::regex &executed, const std::regex &unexecuted, bool isCountBeforeLineNum);
  std::set<int> *parse(std::string file);

 private:
  const std::regex executed;
  const std::regex unexecuted;
  const bool isCountBeforeLineNum;
};
