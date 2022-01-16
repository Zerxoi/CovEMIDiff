#pragma once

#include <string>

#include "Enum.h"

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
        static const enum coverageTool coverageToolId = gcov;
        static const enum coverageTool fileTypeId = gcov;
    }

    namespace unmarkedLabel
    {
        static const enum coverageTool coverageToolId = gcov;
        static const enum coverageTool fileTypeId = llvmcov;
    }
    namespace constArrayInitialization
    {
        static const enum coverageTool coverageToolId = gcov;
        static const enum coverageTool fileTypeId = llvmcov;
    }
    namespace jumpBlock
    {
        static const enum coverageTool coverageToolId = llvmcov;
        static const enum coverageTool fileTypeId = llvmcov;
    }
}