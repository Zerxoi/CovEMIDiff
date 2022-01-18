#pragma once

#include "clang/Tooling/Tooling.h"

#include "CoverageParser.h"

template <typename T>
class EMIFrontendActionFactory : public clang::tooling::FrontendActionFactory
{
public:
    EMIFrontendActionFactory(const llvm::cl::opt<int> &MethodOption, const llvm::cl::opt<std::string> &OutputOption, CoverageParser &Parser)
        : MethodOption(MethodOption), OutputOption(OutputOption), Parser(Parser) {}

    std::unique_ptr<clang::FrontendAction> create() override
    {
        return std::make_unique<T>(MethodOption, OutputOption, Parser);
    }

private:
    int MethodOption;
    const std::string &OutputOption;
    CoverageParser &Parser;
};

// Customize FrontendActionFactory based on Method Option and Output Option
template <typename T>
std::unique_ptr<clang::tooling::FrontendActionFactory> newEMIFrontendActionFactory(const llvm::cl::opt<int> &MethodOption, const llvm::cl::opt<std::string> &OutputOption, CoverageParser &Parser)
{
    return std::unique_ptr<clang::tooling::FrontendActionFactory>(new EMIFrontendActionFactory<T>(MethodOption, OutputOption, Parser));
};