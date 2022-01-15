#pragma once

#include <string>

class DiffReason
{
public:
    explicit DiffReason(int LineNum, int CoverageToolId, int FileTypeId, const std::string &Description, int Count);
    int getLineNum();
    int getCoverageToolId();
    const std::string getCoverageTool();
    int getFileTypeId();
    const std::string getFileType();
    const std::string &getDescription();
    int getCount();

private:
    int LineNum;
    int CoverageToolId;
    int FileTypeId;
    const std::string &Description;
    int Count;
};