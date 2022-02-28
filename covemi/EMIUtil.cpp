#include "EMIUtil.h"

void util::diffExecutedMaps(std::map<int, int> &m1, std::map<int, int> &m2, std::vector<int> &diffLines) {
  for (const auto &[key, value] : m1) {
    if (m2.contains(key) && (value != m2[key])) {
      diffLines.push_back(key);
    }
  }
}

std::filesystem::path util::pathAppend(const std::filesystem::path &path, const std::filesystem::path &append) {
  std::filesystem::path target;
  if (append.empty() || !append.has_root_directory()) {
    target /= path;
  }
  if (!append.empty()) {
    target /= append;
  }
  return target;
}