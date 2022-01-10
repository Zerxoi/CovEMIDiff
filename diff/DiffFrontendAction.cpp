#include "DiffFrontendAction.h"
#include "DiffASTConsumer.h"

DiffFrontendAction::DiffFrontendAction(const std::vector<int> &gcovLines, const std::vector<int> &llvmcovLines) : gcovLines(gcovLines), llvmcovLines(llvmcovLines){};

std::unique_ptr<clang::ASTConsumer> DiffFrontendAction::CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile)
{
    return std::make_unique<DiffASTConsumer>(&Compiler.getASTContext());
}
