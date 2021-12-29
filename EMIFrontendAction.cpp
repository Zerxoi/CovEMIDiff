#include <fstream>
#include "clang/Frontend/CompilerInstance.h"

#include "EMIFrontendAction.h"
#include "EMIASTConsumer.h"

void EMIFrontendAction::EndSourceFileAction()
{
  clang::SourceManager &SM = TheRewriter.getSourceMgr();

  // Write EMI buffer to local file
  std::error_code err;
  std::string filename = ReportName + ".emi";
  llvm::raw_fd_ostream ofs(filename, err);
  TheRewriter.getEditBuffer(SM.getMainFileID()).write(ofs);
  ofs.close();

  llvm::outs() << "EMI file location: " + filename << "\n";
}

std::unique_ptr<clang::ASTConsumer> GCovFrontendAction::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file)
{
  TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
  ReportName = file.str() + ".gcov";
  return std::make_unique<GCovConsumer>(TheRewriter, CI.getASTContext(), file);
}

std::unique_ptr<clang::ASTConsumer> LLVMCovFrontendAction::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file)
{
  TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
  ReportName = file.str() + ".llvm-cov";
  return std::make_unique<LLVMCovConsumer>(TheRewriter, CI.getASTContext(), file);
}
