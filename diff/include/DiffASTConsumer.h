#pragma once

#include "clang/AST/ASTConsumer.h"

#include "DiffASTVisitor.h"
#include "DiffReason.h"

class DiffASTConsumer : public clang::ASTConsumer
{
public:
    explicit DiffASTConsumer(clang::ASTContext *Context, const std::vector<int> &Lines, const int CoverageToolId, const std::string &Filename, const std::vector<DiffParser *> *DiffParserVector, std::vector<DiffReason *> &DiffReasonVector);
    virtual bool HandleTopLevelDecl(clang::DeclGroupRef D) override;

private:
    clang::ASTContext *Context;
    DiffASTVisitor Visitor;
    const std::string Filename;
};