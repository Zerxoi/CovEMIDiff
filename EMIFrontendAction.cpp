#include <fstream>
#include "clang/Frontend/CompilerInstance.h"

#include "EMIFrontendAction.h"
#include "EMIASTConsumer.h"

EMIFrontendAction::EMIFrontendAction(std::string Extension) : Extension(Extension) {}

void EMIFrontendAction::EndSourceFileAction()
{
  clang::SourceManager &SM = TheRewriter.getSourceMgr();

  // Write EMI buffer to local file
  std::error_code err;
  int pos = FileName.find_last_of('.');
  std::string prefix = FileName.substr(0, pos);
  std::string suffix = FileName.substr(pos, FileName.size());
  std::string emi = prefix + Extension + ".emi" + suffix;
  llvm::raw_fd_ostream ofs(emi, err);
  TheRewriter.getEditBuffer(SM.getMainFileID()).write(ofs);
  ofs.close();

  llvm::outs() << "EMI file location: " + emi << "\n";
}

GCovFrontendAction::GCovFrontendAction() : EMIFrontendAction(".gcov") {}

std::unique_ptr<clang::ASTConsumer> GCovFrontendAction::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file)
{
  TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
  FileName = file.str();
  return std::make_unique<GCovConsumer>(TheRewriter, CI.getASTContext(), file);
}

LLVMCovFrontendAction::LLVMCovFrontendAction() : EMIFrontendAction(".llvm-cov") {}

std::unique_ptr<clang::ASTConsumer> LLVMCovFrontendAction::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file)
{
  TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
  FileName = file.str();
  return std::make_unique<LLVMCovConsumer>(TheRewriter, CI.getASTContext(), file);
}
