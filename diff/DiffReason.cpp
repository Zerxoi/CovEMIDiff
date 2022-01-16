#include "Util.h"
#include "DiffReason.h"

DiffReason::DiffReason(int LineNum, const std::string &Description)
    : LineNum(LineNum), Description(Description), Parser(nullptr), Count(0){};

DiffReason::DiffReason(int LineNum, const DiffParser *Parser, int Count)
    : LineNum(LineNum), Description(Parser->getDescription()), Parser(Parser), Count(Count){};

const int DiffReason::getLineNum() const { return LineNum; }

const std::string &DiffReason::getDescription() const { return Description; }

const DiffParser *DiffReason::getParser() const { return Parser; }

const int DiffReason::getCount() const { return Count; }