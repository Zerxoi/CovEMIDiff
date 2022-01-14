#pragma once

#include <string>

class DiffReason
{
public:
    explicit DiffReason(int LineNum, int CoverageToolId, int FileTypeId, const std::string &Description);
    int getLineNum();
    int getCoverageToolId();
    const std::string getCoverageTool();
    int getFileTypeId();
    const std::string getFileType();
    const std::string &getDescription();

private:
    int LineNum;
    int CoverageToolId;
    int FileTypeId;
    const std::string &Description;
};