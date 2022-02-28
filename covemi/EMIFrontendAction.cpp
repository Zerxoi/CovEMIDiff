#include "EMIFrontendAction.h"

#include <filesystem>
#include <fstream>

#include "EMIASTConsumer.h"
#include "EMIConst.h"
#include "EMIUtil.h"
#include "clang/Frontend/CompilerInstance.h"

EMIFrontendAction::EMIFrontendAction(std::string Extension, int MethodOption, const std::string &OutputOption, CoverageParser &Parser)
    : Extension(Extension), MethodOption(MethodOption), OutputOption(OutputOption), Parser(Parser) {}

void EMIFrontendAction::EndSourceFileAction() {
  clang::SourceManager &SM = TheRewriter.getSourceMgr();

  // Write EMI buffer to local file
  std::error_code err;
  std::filesystem::path filePath = FileName;

  std::filesystem::path outputpath = OutputOption;
  std::filesystem::path emiPath = util::pathAppend(filePath.parent_path(), outputpath);

  emiPath /= filePath.stem();
  emiPath += Extension;
  emiPath += filePath.extension();

  std::filesystem::create_directories(emiPath.parent_path());
  std::string path = emiPath.string();
  llvm::raw_fd_ostream ofs(path, err);
  TheRewriter.getEditBuffer(SM.getMainFileID()).write(ofs);
  ofs.close();
  llvm::outs() << "EMI file location: " << path << "\n";
}

GCovFrontendAction::GCovFrontendAction(int MethodOption, const std::string &OutputOption, CoverageParser &Parser)
    : EMIFrontendAction(extension::gcov, MethodOption, OutputOption, Parser) {}

std::unique_ptr<clang::ASTConsumer> GCovFrontendAction::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file) {
  TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
  FileName = file.str();
  return std::make_unique<GCovConsumer>(TheRewriter, CI.getASTContext(), file, MethodOption, Parser);
}

LLVMCovFrontendAction::LLVMCovFrontendAction(int MethodOption, const std::string &OutputOption, CoverageParser &Parser)
    : EMIFrontendAction(extension::llvmcov, MethodOption, OutputOption, Parser) {}

std::unique_ptr<clang::ASTConsumer> LLVMCovFrontendAction::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file) {
  TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
  FileName = file.str();
  return std::make_unique<LLVMCovConsumer>(TheRewriter, CI.getASTContext(), file, MethodOption, Parser);
}
