#pragma once

#include "clang/Tooling/Tooling.h"

// Customize FrontendActionFactory based on Method Option and Output Option
template <typename T>
std::unique_ptr<clang::tooling::FrontendActionFactory> EMINewFrontendActionFactory(const llvm::cl::opt<int> &MethodOption, const llvm::cl::opt<std::string> &OutputOption)
{
    class SimpleFrontendActionFactory : public clang::tooling::FrontendActionFactory
    {
    public:
        SimpleFrontendActionFactory(const llvm::cl::opt<int> &MethodOption, const llvm::cl::opt<std::string> &OutputOption)
            : MethodOption(MethodOption), OutputOption(OutputOption)
        {
        }

        std::unique_ptr<clang::FrontendAction> create() override
        {
            return std::make_unique<T>(MethodOption, OutputOption);
        }

    private:
        int MethodOption;
        const std::string &OutputOption;
    };

    return std::unique_ptr<clang::tooling::FrontendActionFactory>(new SimpleFrontendActionFactory(MethodOption, OutputOption));
};