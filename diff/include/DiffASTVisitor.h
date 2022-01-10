#pragma once

#include "clang/AST/RecursiveASTVisitor.h"

class DiffASTVisitor : public clang::RecursiveASTVisitor<DiffASTVisitor>
{
public:
    explicit DiffASTVisitor(clang::ASTContext *Context);

private:
    clang::ASTContext *Context;
};