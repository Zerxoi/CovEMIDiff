#pragma once
#include <filesystem>

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"

#include "DiffReason.h"
#include "DiffParser.h"

class DiffFrontendAction : public clang::ASTFrontendAction
{
public:
    explicit DiffFrontendAction(const std::vector<int> &gcovLines, const std::vector<int> &llvmcovLines, const std::filesystem::path &DirPath, const std::vector<DiffParser *> *DiffParserVector);
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile) override;
    virtual void EndSourceFileAction() override;

private:
    const std::vector<int> &gcovLines;
    int gcovIndex = 0;
    const std::vector<int> &llvmcovLines;
    int llvmcovIndex = 0;
    const std::filesystem::path &DirPath;
    int CoverageToolId;
    const std::vector<DiffParser *> *DiffParserVector;
    std::vector<DiffReason *> DiffReasonVector;
};