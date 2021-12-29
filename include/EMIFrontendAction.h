#pragma once
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "EMIASTConsumer.h"

// For each source file provided to the tool, a new FrontendAction is created.
class EMIFrontendAction : public clang::ASTFrontendAction
{
protected:
    clang::Rewriter TheRewriter;
    std::string ReportName;

public:
    EMIFrontendAction() {}

    // Write pruned source(EMI) to the local file.
    // The name of local file is <report-name>.emi, the format of <report-name>
    //  is <source-name>.<extension>, Where <source-name> is the file name of
    // the source code and the <extension> of gcov and llvm-cov are gcov and
    // llvm-cov respectively
    void EndSourceFileAction() override;

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override = 0;
};

class GCovFrontendAction : public EMIFrontendAction
{
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override;
};

class LLVMCovFrontendAction : public EMIFrontendAction
{
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override;
};