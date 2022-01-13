#pragma once
#include <filesystem>

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"

#include "DiffParser.h"

class DiffFrontendAction : public clang::ASTFrontendAction
{
public:
    explicit DiffFrontendAction(const std::vector<int> &gcovLines, const std::vector<int> &llvmcovLines, const std::filesystem::path &DirPath, const std::vector<DiffParser *> *DiffParserVector);
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile) override;
    virtual void EndSourceFileAction() override;

private:
    const std::vector<int> &gcovLines;
    const std::vector<int> &llvmcovLines;
    const std::filesystem::path &DirPath;
    int CoverageToolId;
    std::map<int, std::string> ReasonMap;
    const std::vector<DiffParser *> *DiffParserVector;
};