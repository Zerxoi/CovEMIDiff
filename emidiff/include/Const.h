#pragma once

#include <string>

#include "Enum.h"
namespace extension
{
    static const std::string gcov = ".gcov";
    static const std::string llvmcov = ".llvm-cov";
}

namespace reason
{
    namespace description
    {
        static const std::string skippable = "Skippable";
        static const std::string unparsed = "Unparsed";
        static const std::string terminated = "Terminated";
        static const std::string ifOptimize = "If Optimize";
        static const std::string unmarkedLabel = "Unmarked Label";
        static const std::string constArrayInitialization = "Const Array Initialization";
        static const std::string jumpBlock = "Jump Block";
    }
}

namespace parser
{
    namespace ifOptimize
    {
        static const enum coverageTool coverageTool = gcov;
        static const enum coverageTool fileType = gcov;
    }

    namespace unmarkedLabel
    {
        static const enum coverageTool coverageTool = gcov;
        static const enum coverageTool fileType = llvmcov;
    }
    namespace constArrayInitialization
    {
        static const enum coverageTool coverageTool = gcov;
        static const enum coverageTool fileType = gcov;
    }
    namespace jumpBlock
    {
        static const enum coverageTool coverageTool = llvmcov;
        static const enum coverageTool fileType = llvmcov;
    }
}