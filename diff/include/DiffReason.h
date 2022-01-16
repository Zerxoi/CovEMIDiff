#pragma once

#include <string>

#include "DiffParser.h"

// The class that stores the cause of the Diff
class DiffReason
{
public:
    explicit DiffReason(int LineNum, const std::string &Description);
    explicit DiffReason(int LineNum, const DiffParser *Parser, int Count);
    const int getLineNum() const;
    const std::string &getDescription() const;
    const DiffParser *getParser() const;
    const int getCount() const;

private:
    const int LineNum;
    const std::string &Description;
    const DiffParser *Parser;
    int Count;
};