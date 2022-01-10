#pragma once

#include <filesystem>

#include "clang/Tooling/Tooling.h"

class DiffFrontendActionFactory : public clang::tooling::FrontendActionFactory
{
public:
    DiffFrontendActionFactory(const std::string &gcovFile, const std::string &llvmcovFile, const std::string& DirOption);

    std::unique_ptr<clang::FrontendAction> create() override;

private:
    std::vector<int> gcovLines;
    std::vector<int> llvmcovLines;
    std::filesystem::path DirPath;
};

std::unique_ptr<clang::tooling::FrontendActionFactory> newDiffFrontendActionFactory(const std::string &gcovFile, const std::string &llvmcovFile, const std::string &DirOption);