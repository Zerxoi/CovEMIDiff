#pragma once
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "EMIASTConsumer.h"

// For each source file provided to the tool, a new FrontendAction is created.
class EMIFrontendAction : public clang::ASTFrontendAction
{
protected:
    const std::string Extension;
    clang::Rewriter TheRewriter;
    std::string FileName;
    int MethodOption;

public:
    EMIFrontendAction(std::string Extension, int MethodOption);

    // Write pruned source(EMI) to the local file.
    // The format of local file is <source-name>.<extension>.emi.<source-extension>, 
    // where <extension> is determined based on the code coverage tool used
    void EndSourceFileAction() override;

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override = 0;
};

class GCovFrontendAction : public EMIFrontendAction
{
public:
    GCovFrontendAction(int MethodOption);
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override;
};

class LLVMCovFrontendAction : public EMIFrontendAction
{
public:
    LLVMCovFrontendAction(int MethodOption);
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override;
};