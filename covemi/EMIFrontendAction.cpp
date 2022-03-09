#include "EMIFrontendAction.h"

#include <filesystem>
#include <fstream>

#include "EMIASTConsumer.h"
#include "EMIConst.h"
#include "EMIUtil.h"
#include "clang/Frontend/CompilerInstance.h"

EMIFrontendAction::EMIFrontendAction(std::string Extension, const std::string &CoverageToolVersion, int MethodOption, const std::string &OutputOption)
    : Extension(Extension), CoverageToolVersion(CoverageToolVersion), MethodOption(MethodOption), OutputOption(OutputOption) {}

void EMIFrontendAction::EndSourceFileAction() {
  clang::SourceManager &SM = TheRewriter.getSourceMgr();

  // Write EMI buffer to local file
  std::error_code err;
  std::filesystem::path filePath = FileName;

  std::filesystem::path outputpath = OutputOption;
  std::filesystem::path emiPath = util::pathAppend(filePath.parent_path(), outputpath);

  emiPath /= filePath.stem();
  emiPath += Extension;
  if (MethodOption == method::pre) {
    emiPath += extension::pre;
  } else if (MethodOption == method::post) {
    emiPath += extension::post;
  }
  if (!CoverageToolVersion.empty()) {
    emiPath += "." + CoverageToolVersion;
  }
  emiPath += filePath.extension();
  std::filesystem::create_directories(emiPath.parent_path());
  std::string path = emiPath.lexically_normal();
  llvm::raw_fd_ostream ofs(path, err);
  TheRewriter.getEditBuffer(SM.getMainFileID()).write(ofs);
  ofs.close();
}

GCovFrontendAction::GCovFrontendAction(const std::string &CoverageToolVersion, int MethodOption, const std::string &OutputOption)
    : EMIFrontendAction(extension::gcov, CoverageToolVersion, MethodOption, OutputOption) {}

std::unique_ptr<clang::ASTConsumer> GCovFrontendAction::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file) {
  TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
  FileName = file.str();
  return std::make_unique<GCovConsumer>(TheRewriter, CI.getASTContext(), file, CoverageToolVersion, MethodOption);
}

LLVMCovFrontendAction::LLVMCovFrontendAction(const std::string &CoverageToolVersion, int MethodOption, const std::string &OutputOption)
    : EMIFrontendAction(extension::llvmcov, CoverageToolVersion, MethodOption, OutputOption) {}

std::unique_ptr<clang::ASTConsumer> LLVMCovFrontendAction::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file) {
  TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
  FileName = file.str();
  return std::make_unique<LLVMCovConsumer>(TheRewriter, CI.getASTContext(), file, CoverageToolVersion, MethodOption);
}
