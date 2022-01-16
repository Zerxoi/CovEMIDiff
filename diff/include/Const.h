#pragma once

#include <string>

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
        static const int coverageToolId = 0;
        static const int fileTypeId = 0;
    }

    namespace unmarkedLabel
    {
        static const int coverageToolId = 0;
        static const int fileTypeId = 1;
    }
    namespace constArrayInitialization
    {
        static const int coverageToolId = 0;
        static const int fileTypeId = 0;
    }
    namespace jumpBlock
    {
        static const int coverageToolId = 1;
        static const int fileTypeId = 1;
    }
}