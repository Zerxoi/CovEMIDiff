#pragma once

#include "clang/AST/ASTConsumer.h"

#include "DiffASTVisitor.h"

class DiffASTConsumer : public clang::ASTConsumer
{
public:
    explicit DiffASTConsumer(clang::ASTContext *Context, const std::string &Filename, const std::vector<int> &Lines);
    virtual bool HandleTopLevelDecl(clang::DeclGroupRef D) override;

private:
    clang::ASTContext *Context;
    DiffASTVisitor Visitor;
    std::string Filename;
};