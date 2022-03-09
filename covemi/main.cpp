#include <algorithm>
#include <fstream>
#include <string>

#include "EMIConst.h"
#include "EMIFrontendAction.h"
#include "EMIFrontendActionFactory.h"
#include "EMIUtil.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

static llvm::cl::OptionCategory EMIOptionCategory("EMI Options");
static llvm::cl::opt<std::string> CoverageToolOption("t", llvm::cl::desc("Coverage tool option"), llvm::cl::cat(EMIOptionCategory));
static llvm::cl::opt<std::string> CoverageToolVersionOption("v", llvm::cl::desc("Coverage tool version option"), llvm::cl::cat(EMIOptionCategory));
static llvm::cl::opt<int> MethodOption("m", llvm::cl::desc("EMI prune method option"), llvm::cl::cat(EMIOptionCategory));
static llvm::cl::opt<std::string> OutputOption("o", llvm::cl::desc("Output option"), llvm::cl::cat(EMIOptionCategory));

int main(int argc, const char *argv[]) {
  clang::tooling::CommonOptionsParser op(argc, argv, EMIOptionCategory);
  const auto &sources = op.getSourcePathList();
  if (sources.size() != 1) {
    llvm::outs() << *argv << " can only handle one file argument!\n";
    return 1;
  }
  clang::tooling::ClangTool Tool(op.getCompilations(), sources);

  std::string &CoverageTool = CoverageToolOption.getValue();
  std::transform(CoverageTool.begin(), CoverageTool.end(), CoverageTool.begin(), [](unsigned char c) { return std::tolower(c); });

  // ClangTool::run accepts a FrontendActionFactory, which is then used to
  // create new objects implementing the FrontendAction interface.
  if (CoverageTool == tool::gcov) {
    Tool.run(newEMIFrontendActionFactory<GCovFrontendAction>(CoverageToolVersionOption, MethodOption, OutputOption).get());
  } else if (CoverageTool == tool::llvmcov) {
    Tool.run(newEMIFrontendActionFactory<LLVMCovFrontendAction>(CoverageToolVersionOption, MethodOption, OutputOption).get());
  } else {
    throw std::runtime_error("Please specify correct coverage tool options, gcov or llvm-cov.");
  }
}