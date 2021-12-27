#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "EMIFrontendAction.h"

static llvm::cl::OptionCategory ClangToolCategory("Clang Tool Options");

int Function(int argc, const char **argv)
{
  clang::tooling::CommonOptionsParser op(argc, argv, ClangToolCategory);
  clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());

  // ClangTool::run accepts a FrontendActionFactory, which is then used to
  // create new objects implementing the FrontendAction interface. Here we use
  // the helper newFrontendActionFactory to create a default factory that will
  // return a new MyFrontendAction object every time.
  // To further customize this, we could create our own factory class.
  return Tool.run(clang::tooling::newFrontendActionFactory<EMIFrontendAction>().get());
}