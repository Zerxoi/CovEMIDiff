#pragma once

#include <regex>
#include <string>

namespace extension
{
    static const std::string gcov = ".gcov";
    static const std::string llvmcov = ".llvm-cov";
}

namespace parser
{
    namespace gcov
    {
        static const std::regex executed = std::regex("^\\s*(\\d+):\\s*(\\d+):.*$");
        static const std::regex unexecuted = std::regex("^\\s*#####:\\s*(\\d+):.*$");
        static const bool isCountBeforeLineNum = true;
    }

    namespace llvmcov
    {
        static const std::regex executed = std::regex("^\\s*(\\d+)\\|\\s*([1-9]\\d*)\\|.*$");
        static const std::regex unexecuted = std::regex("^\\s*(\\d+)\\|\\s*0\\|.*$");
        static const bool isCountBeforeLineNum = false;
    }
}