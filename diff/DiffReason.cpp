#include "Util.h"
#include "DiffReason.h"

DiffReason::DiffReason(int LineNum, int CoverageToolId, int FileTypeId, const std::string &Description)
    : LineNum(LineNum), CoverageToolId(CoverageToolId), FileTypeId(FileTypeId), Description(Description) {}

int DiffReason::getLineNum() { return LineNum; }

int DiffReason::getCoverageToolId() { return CoverageToolId; }

const std::string DiffReason::getCoverageTool() { return util::idToString(CoverageToolId); }

int DiffReason::getFileTypeId() { return FileTypeId; }

const std::string DiffReason::getFileType() { return util::idToString(FileTypeId); }

const std::string &DiffReason::getDescription() { return Description; }
