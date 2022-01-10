#pragma once

#include "clang/AST/ASTConsumer.h"

#include "DiffASTVisitor.h"

class DiffASTConsumer : public clang::ASTConsumer
{
public:
    explicit DiffASTConsumer(clang::ASTContext *Context);

    virtual void HandleTranslationUnit(clang::ASTContext &Context) override;

private:
    DiffASTVisitor Visitor;
};