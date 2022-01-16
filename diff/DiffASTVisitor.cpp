#include "Const.h"
#include "DiffReason.h"
#include "DiffASTVisitor.h"

DiffASTVisitor::DiffASTVisitor(clang::ASTContext *Context, const std::vector<int> &Lines, const int Id, const std::vector<DiffParser *> *DiffParserVector, std::vector<DiffReason *> &DiffReasonVector)
    : Context(Context), Lines(Lines), Index(0), CoverageToolId(Id), DiffParserVector(DiffParserVector), DiffReasonVector(DiffReasonVector){};

bool DiffASTVisitor::isSkippable(clang::Stmt *s)
{
    // Can be skipped only if the statement is a declaration statement and the statement
    // does not contain initialization
    if (auto declStmt = clang::dyn_cast<clang::DeclStmt>(s))
    {
        for (auto decl : declStmt->decls())
        {
            if (auto *varDecl = clang::dyn_cast<clang::VarDecl>(decl))
            {
                if (varDecl->hasInit())
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool DiffASTVisitor::VisitStmt(clang::Stmt *s)
{
    int LineNum = Lines[Index];
    if (Index < Lines.size() && LineNum == Context->getSourceManager().getSpellingLineNumber(s->getBeginLoc()))
    {

        bool parsed = false;
        for (auto diffParser : *DiffParserVector)
        {
            if (diffParser->getFileTypeId() == CoverageToolId && diffParser->parse(s, Context))
            {
                parsed = true;
                DiffReasonVector.push_back(new DiffReason(LineNum, diffParser, diffParser->getCount()));
                break;
            }
        }
        if (!parsed)
        {
            if (isSkippable(s))
            {
                DiffReasonVector.push_back(new DiffReason(LineNum, reason::description::skippable));
            }
            else
            {
                DiffReasonVector.push_back(new DiffReason(LineNum, reason::description::unparsed));
            }
        }
        Index++;
    }
    return true;
}