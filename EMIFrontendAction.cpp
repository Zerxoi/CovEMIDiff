#include "clang/Frontend/CompilerInstance.h"

#include "EMIFrontendAction.h"
#include "EMIASTConsumer.h"

void EMIFrontendAction::EndSourceFileAction()
{
  // clang::SourceManager &SM = TheRewriter.getSourceMgr();
  // llvm::outs() << "** EndSourceFileAction for: "
  //              << SM.getFileEntryForID(SM.getMainFileID())->getName() << "\n";

  // // Now emit the rewritten buffer.
  // TheRewriter.getEditBuffer(SM.getMainFileID()).write(llvm::outs());
}

std::unique_ptr<clang::ASTConsumer> EMIFrontendAction::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file)
{
  TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
  return std::make_unique<clang::ASTConsumer>(*CreateEMIASTConsumer(TheRewriter, CI.getASTContext(), file));
}



EMIASTConsumer *GCovFrontendAction::CreateEMIASTConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename)
{
  return new GCovConsumer(R, Context, filename);
}

EMIASTConsumer *LLVMCovFrontendAction::CreateEMIASTConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename)
{
  return new LLVMCovConsumer(R, Context, filename);
}
