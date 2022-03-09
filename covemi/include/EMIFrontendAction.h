#pragma once

#include "EMIASTConsumer.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"

// For each source file provided to the tool, a new FrontendAction is created.
class EMIFrontendAction : public clang::ASTFrontendAction {
 public:
  EMIFrontendAction(std::string Extension, const std::string &CoverageToolVersion, int MethodOption, const std::string &OutputOption);

  // Write pruned source(EMI) to the local file.
  // The format of local file is <source-name>.<extension>.emi.<source-extension>,
  // where <extension> is determined based on the code coverage tool used
  void EndSourceFileAction() override;

  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override = 0;

 protected:
  const std::string Extension;
  clang::Rewriter TheRewriter;
  std::string FileName;
  const std::string &CoverageToolVersion;
  int MethodOption;
  const std::string &OutputOption;
};

class GCovFrontendAction : public EMIFrontendAction {
 public:
  GCovFrontendAction(const std::string &CoverageToolVersion, int MethodOption, const std::string &OutputOption);
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override;
};

class LLVMCovFrontendAction : public EMIFrontendAction {
 public:
  LLVMCovFrontendAction(const std::string &CoverageToolVersion, int MethodOption, const std::string &OutputOption);
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override;
};