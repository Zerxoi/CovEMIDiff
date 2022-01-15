#include "DiffReason.h"
#include "DiffASTVisitor.h"

DiffASTVisitor::DiffASTVisitor(clang::ASTContext *Context, const std::vector<int> &Lines, int &Index, const int Id, const std::vector<DiffParser *> *DiffParserVector, std::vector<DiffReason *> &DiffReasonVector)
    : Context(Context), Lines(Lines), Index(Index), CoverageToolId(Id), DiffParserVector(DiffParserVector), DiffReasonVector(DiffReasonVector){};

bool DiffASTVisitor::VisitStmt(clang::Stmt *s)
{
    if (Index < Lines.size() && Lines[Index] == Context->getSourceManager().getSpellingLineNumber(s->getBeginLoc()))
    {
        for (auto diffParser : *DiffParserVector)
        {
            if (diffParser->getFileTypeId() == CoverageToolId && diffParser->parse(s, Context))
            {
                DiffReasonVector.push_back(new DiffReason(Lines[Index], diffParser->getCoverageToolId(), diffParser->getFileTypeId(), diffParser->getDescription(), diffParser->getCount()));
                break;
            }
        }
        Index++;
    }
    return true;
}