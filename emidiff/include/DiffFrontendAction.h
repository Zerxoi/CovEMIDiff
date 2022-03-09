#pragma once
#include <cppconn/connection.h>

#include <filesystem>

#include "DiffEnum.h"
#include "DiffReason.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"

class DiffFrontendAction : public clang::ASTFrontendAction {
 public:
  explicit DiffFrontendAction(const std::vector<int> &Lines, const int DiffIdOption, const std::vector<DiffParser *> *DiffParserVector, sql::ConnectOptionsMap &ConnProperties);
  virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile) override;
  virtual void EndSourceFileAction() override;

 private:
  const std::vector<int> &Lines;
  const int DiffIdOption;
  enum coverageTool CoverageTool;
  const std::vector<DiffParser *> *DiffParserVector;
  std::vector<DiffReason *> DiffReasonVector;
  sql::ConnectOptionsMap &ConnProperties;
};