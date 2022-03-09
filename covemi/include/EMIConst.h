#pragma once

#include <regex>
#include <string>

namespace extension {
static const std::string gcov = ".gcov";
static const std::string llvmcov = ".llvm-cov";
static const std::string pre = ".pre";
static const std::string post = ".post";
}  // namespace extension

namespace parser {
namespace gcov {
static const std::regex executed = std::regex("^\\s*(\\d+)\\*?:\\s*(\\d+):.*$");
static const std::regex unexecuted = std::regex("^\\s*#####:\\s*(\\d+):.*$");
static const bool isCountBeforeLineNum = true;
}  // namespace gcov

namespace llvmcov {
static const std::regex executed = std::regex("^\\s*(\\d+)\\|\\s*([1-9]\\d*)\\|.*$");
static const std::regex unexecuted = std::regex("^\\s*(\\d+)\\|\\s*0\\|.*$");
static const bool isCountBeforeLineNum = false;
}  // namespace llvmcov
}  // namespace parser

namespace method {
enum emiMethod {
  pre,
  post
};
}

namespace tool {
static const std::string gcov = "gcov";
static const std::string llvmcov = "llvm-cov";
}  // namespace tool