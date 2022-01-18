#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CommonOptionsParser.h"

#include "DiffFrontendActionFactory.h"

static llvm::cl::OptionCategory DiffOptionCategory("Diff Options");
static llvm::cl::opt<std::string> DirOption("d", llvm::cl::desc("Diff output directory option"), llvm::cl::cat(DiffOptionCategory));

int main(int argc, const char *argv[])
{
    clang::tooling::CommonOptionsParser op(argc, argv, DiffOptionCategory);
    const auto &sources = op.getSourcePathList();
    if (sources.size() != 2)
    {
        llvm::outs() << *argv << " compare the difference between two EMI programs.\n"
                     << "Please enter 2 file path parameters!\n";
        return 1;
    }
    clang::tooling::ClangTool Tool(op.getCompilations(), sources);
    Tool.run(newDiffFrontendActionFactory(sources[0], sources[1], DirOption).get());
}