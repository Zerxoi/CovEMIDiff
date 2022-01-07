#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "EMINewFrontendActionFactory.h"
#include "EMIFrontendAction.h"

static llvm::cl::OptionCategory EMIOptionCategory("EMI Options");
static llvm::cl::opt<int> MethodOption("m", llvm::cl::desc("EMI prune method"), llvm::cl::cat(EMIOptionCategory));
static llvm::cl::opt<std::string> OutputOption("o", llvm::cl::desc("Explicitly specify a output directory"), llvm::cl::cat(EMIOptionCategory));

int main(int argc, const char *argv[])
{
    clang::tooling::CommonOptionsParser op(argc, argv, EMIOptionCategory);
    clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());

    // ClangTool::run accepts a FrontendActionFactory, which is then used to
    // create new objects implementing the FrontendAction interface.
    Tool.run(EMINewFrontendActionFactory<GCovFrontendAction>(MethodOption, OutputOption).get());
    Tool.run(EMINewFrontendActionFactory<LLVMCovFrontendAction>(MethodOption, OutputOption).get());
}