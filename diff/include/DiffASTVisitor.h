#pragma once

#include "clang/AST/RecursiveASTVisitor.h"

class DiffASTVisitor : public clang::RecursiveASTVisitor<DiffASTVisitor>
{
public:
    explicit DiffASTVisitor(clang::ASTContext *Context, const std::vector<int> &Lines);
    virtual bool VisitStmt(clang::Stmt *s);

private:
    clang::ASTContext *Context;
    const std::vector<int> &Lines;
    int Index = 0;
};