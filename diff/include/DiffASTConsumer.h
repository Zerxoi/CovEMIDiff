#pragma once

#include "clang/AST/ASTConsumer.h"

#include "DiffASTVisitor.h"

class DiffASTConsumer : public clang::ASTConsumer
{
public:
    explicit DiffASTConsumer(const clang::ASTContext *Context, const std::string &Filename, const std::vector<int> &Lines);
    virtual bool HandleTopLevelDecl(clang::DeclGroupRef D) override;

private:
    const clang::ASTContext *Context;
    DiffASTVisitor Visitor;
    std::string Filename;
};