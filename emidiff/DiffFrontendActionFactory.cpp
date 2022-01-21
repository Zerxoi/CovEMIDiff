#include <filesystem>

#include "EMIDiff.h"
#include "DiffFrontendAction.h"
#include "DiffFrontendActionFactory.h"

DiffFrontendActionFactory::DiffFrontendActionFactory(const std::string &gcovFile, const std::string &llvmcovFile, const int TaskIdOption, const int MethodOption, sql::ConnectOptionsMap &ConnProperties)
    : TaskIdOption(TaskIdOption), MethodOption(MethodOption), ConnProperties(ConnProperties)
{
    EMIDiff(gcovFile, llvmcovFile, gcovLines, llvmcovLines);
    if (TaskIdOption <= 0)
    {
        throw std::runtime_error("Please give a valid Task Id value");
    }
    if (MethodOption != 0 && MethodOption != 1)
    {
        throw std::runtime_error("Unable to handle this MethodOption");
    }
    DiffParserVector = createDiffParserVector();
}

std::unique_ptr<clang::FrontendAction> DiffFrontendActionFactory::create()
{
    return std::make_unique<DiffFrontendAction>(gcovLines, llvmcovLines, TaskIdOption, MethodOption, DiffParserVector, ConnProperties);
}

std::unique_ptr<clang::tooling::FrontendActionFactory> newDiffFrontendActionFactory(const std::string &gcovFile, const std::string &llvmcovFile, const int TaskIdOption, const int MethodOption, sql::ConnectOptionsMap &ConnProperties)
{
    return std::unique_ptr<clang::tooling::FrontendActionFactory>(new DiffFrontendActionFactory(gcovFile, llvmcovFile, TaskIdOption, MethodOption, ConnProperties));
}