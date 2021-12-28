#pragma once
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "EMIASTConsumer.h"

// For each source file provided to the tool, a new FrontendAction is created.
class EMIFrontendAction : public clang::ASTFrontendAction
{
private:
    clang::Rewriter TheRewriter;

public:
    EMIFrontendAction() {}
    void EndSourceFileAction() override;

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override;

    virtual EMIASTConsumer *CreateEMIASTConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename) = 0;
};

class GCovFrontendAction : public EMIFrontendAction
{
public:
    EMIASTConsumer *CreateEMIASTConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename);
};

class LLVMCovFrontendAction : public EMIFrontendAction
{
public:
    EMIASTConsumer *CreateEMIASTConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename);
};