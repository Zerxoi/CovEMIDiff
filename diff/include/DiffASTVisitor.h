#pragma once

#include "clang/AST/RecursiveASTVisitor.h"

class DiffASTVisitor : public clang::RecursiveASTVisitor<DiffASTVisitor>
{
public:
    explicit DiffASTVisitor(const clang::ASTContext *Context, const std::vector<int> &Lines);
    virtual bool VisitStmt(clang::Stmt *s);

private:
    const clang::ASTContext *Context;
    const std::vector<int> &Lines;
};