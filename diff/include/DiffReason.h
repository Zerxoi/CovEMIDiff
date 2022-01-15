#pragma once

#include <string>

// The class that stores the cause of the Diff
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
    int CoverageToolId; // Diff bug is at that coverage tool's ID
    int FileTypeId;     // In which coverage tool's EMI file type can the Diff bug be found
    const std::string &Description;
    int Count;
};