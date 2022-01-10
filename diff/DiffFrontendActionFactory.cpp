#include <filesystem>

#include "DiffFrontendActionFactory.h"
#include "DiffFrontendAction.h"
#include "EMIDiff.h"

DiffFrontendActionFactory::DiffFrontendActionFactory(const std::string &gcovFile, const std::string &llvmcovFile, const std::string &DirOption)
{
    EMIDiff(gcovFile, llvmcovFile, gcovLines, llvmcovLines);
    if (!DirOption.empty())
    {
        std::filesystem::path path = DirOption;
        if (!path.has_root_directory())
        {
            DirPath /= std::filesystem::current_path();
        }
        DirPath /= path;
    }
    else
    {
        DirPath /= std::filesystem::current_path();
    }
}

std::unique_ptr<clang::FrontendAction> DiffFrontendActionFactory::create()
{
    return std::make_unique<DiffFrontendAction>(gcovLines, llvmcovLines, DirPath);
}

std::unique_ptr<clang::tooling::FrontendActionFactory> newDiffFrontendActionFactory(const std::string &gcovFile, const std::string &llvmcovFile, const std::string &DirOption)
{
    return std::unique_ptr<clang::tooling::FrontendActionFactory>(new DiffFrontendActionFactory(gcovFile, llvmcovFile, DirOption));
}