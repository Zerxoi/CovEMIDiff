#include "clang/Frontend/CompilerInstance.h"

#include "EMIFrontendAction.h"
#include "EMIASTConsumer.h"

std::unique_ptr<clang::ASTConsumer>
EMIFrontendAction::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file)
{
  llvm::outs() << "** Creating AST consumer for: " << file << "\n";
  TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());

  return std::make_unique<EMIASTConsumer>(TheRewriter, CI.getASTContext(), file);
}

void EMIFrontendAction::EndSourceFileAction()
{
  // clang::SourceManager &SM = TheRewriter.getSourceMgr();
  // llvm::outs() << "** EndSourceFileAction for: "
  //              << SM.getFileEntryForID(SM.getMainFileID())->getName() << "\n";

  // // Now emit the rewritten buffer.
  // TheRewriter.getEditBuffer(SM.getMainFileID()).write(llvm::outs());
}