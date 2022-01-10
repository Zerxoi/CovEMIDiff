#include "DiffASTVisitor.h"

DiffASTVisitor::DiffASTVisitor(const clang::ASTContext *Context, const std::vector<int> &Lines)
    : Context(Context), Lines(Lines){};

bool DiffASTVisitor::VisitStmt(clang::Stmt *s)
{
    return true;
}