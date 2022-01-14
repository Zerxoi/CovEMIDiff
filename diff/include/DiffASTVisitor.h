#pragma once

#include "clang/AST/RecursiveASTVisitor.h"

#include "DiffParser.h"
#include "DiffReason.h"

class DiffASTVisitor : public clang::RecursiveASTVisitor<DiffASTVisitor>
{
public:
    explicit DiffASTVisitor(clang::ASTContext *Context, const std::vector<int> &Lines, const int Id, const std::vector<DiffParser *> *DiffParserVector, std::vector<DiffReason *> &DiffReasonVector);
    virtual bool VisitStmt(clang::Stmt *s);

private:
    clang::ASTContext *Context;
    const std::vector<int> &Lines;
    int Index = 0;
    const int CoverageToolId;
    const std::vector<DiffParser *> *DiffParserVector;
    std::vector<DiffReason *> &DiffReasonVector;
};