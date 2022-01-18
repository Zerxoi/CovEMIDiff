#include <fstream>

#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "EMIUtil.h"
#include "EMIConst.h"
#include "CoverageParser.h"
#include "EMIFrontendAction.h"
#include "EMIFrontendActionFactory.h"

static llvm::cl::OptionCategory EMIOptionCategory("EMI Options");
// EMI prune method option - Specify the method of EMI pruning
static llvm::cl::opt<int> MethodOption("m", llvm::cl::desc("EMI prune method"), llvm::cl::cat(EMIOptionCategory));
// Output directory option - Specify the output directory of the generated EMI file
static llvm::cl::opt<std::string> OutputOption("o", llvm::cl::desc("Explicitly specify a output directory"), llvm::cl::cat(EMIOptionCategory));

int main(int argc, const char *argv[])
{
    clang::tooling::CommonOptionsParser op(argc, argv, EMIOptionCategory);
    const auto &sources = op.getSourcePathList();
    if (sources.size() != 1)
    {
        llvm::outs() << *argv << " can only handle one file argument!\n";
        return 1;
    }
    clang::tooling::ClangTool Tool(op.getCompilations(), sources);

    CoverageParser gcovParser = CoverageParser(parser::gcov::executed, parser::gcov::unexecuted, parser::gcov::isCountBeforeLineNum);
    CoverageParser llvmcovParser = CoverageParser(parser::llvmcov::executed, parser::llvmcov::unexecuted, parser::llvmcov::isCountBeforeLineNum);

    // ClangTool::run accepts a FrontendActionFactory, which is then used to
    // create new objects implementing the FrontendAction interface.
    Tool.run(newEMIFrontendActionFactory<GCovFrontendAction>(MethodOption, OutputOption, gcovParser).get());
    Tool.run(newEMIFrontendActionFactory<LLVMCovFrontendAction>(MethodOption, OutputOption, llvmcovParser).get());

    std::vector<int> diffLines;
    util::diffExecutedMaps(gcovParser.getExecutedMap(), llvmcovParser.getExecutedMap(), diffLines);
    if (diffLines.size() > 0)
    {
        std::string append = OutputOption;
        auto path = util::pathAppend(std::filesystem::path(sources[0]).parent_path(), append) / "diffsLines";
        std::ofstream ofs(path);
        for (auto line : diffLines)
        {
            ofs << line << "\n";
        }
        ofs.close();
        llvm::outs() << "Diff lines file location: " + path.string();
    }
}