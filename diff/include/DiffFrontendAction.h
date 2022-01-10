#pragma once

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"

class DiffFrontendAction : public clang::ASTFrontendAction
{
public:
    explicit DiffFrontendAction(const std::vector<int> &gcovLines, const std::vector<int> &llvmcovLines);

    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile) override;

private:
    const std::vector<int> &gcovLines;
    const std::vector<int> &llvmcovLines;
};