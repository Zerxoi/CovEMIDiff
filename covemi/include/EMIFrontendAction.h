#pragma once
#include "CoverageParser.h"
#include "EMIASTConsumer.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"

// For each source file provided to the tool, a new FrontendAction is created.
class EMIFrontendAction : public clang::ASTFrontendAction {
 public:
  EMIFrontendAction(std::string Extension, int MethodOption, const std::string &OutputOption, CoverageParser &Parser);

  // Write pruned source(EMI) to the local file.
  // The format of local file is <source-name>.<extension>.emi.<source-extension>,
  // where <extension> is determined based on the code coverage tool used
  void EndSourceFileAction() override;

  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override = 0;

 protected:
  const std::string Extension;
  clang::Rewriter TheRewriter;
  std::string FileName;
  int MethodOption;
  const std::string &OutputOption;
  CoverageParser &Parser;
};

class GCovFrontendAction : public EMIFrontendAction {
 public:
  GCovFrontendAction(int MethodOption, const std::string &OutputOption, CoverageParser &Parser);
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override;
};

class LLVMCovFrontendAction : public EMIFrontendAction {
 public:
  LLVMCovFrontendAction(int MethodOption, const std::string &OutputOption, CoverageParser &Parser);
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override;
};