#include "DiffFrontendActionFactory.h"
#include "DiffFrontendAction.h"
#include "EMIDiff.h"

DiffFrontendActionFactory::DiffFrontendActionFactory(const std::string &gcovFile, const std::string &llvmcovFile)
{
    EMIDiff(gcovFile, llvmcovFile, gcovLines, llvmcovLines);
}

std::unique_ptr<clang::FrontendAction> DiffFrontendActionFactory::create()
{
    return std::make_unique<DiffFrontendAction>(gcovLines, llvmcovLines);
}

std::unique_ptr<clang::tooling::FrontendActionFactory> newDiffFrontendActionFactory(const std::string &gcovFile, const std::string &llvmcovFile)
{
    return std::unique_ptr<clang::tooling::FrontendActionFactory>(new DiffFrontendActionFactory(gcovFile, llvmcovFile));
};