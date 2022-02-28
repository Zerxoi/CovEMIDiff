#pragma once
#include "EMIASTVisitor.h"
#include "clang/AST/ASTConsumer.h"

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class EMIASTConsumer : public clang::ASTConsumer {
 public:
  EMIASTConsumer(clang::ASTContext &Context, std::string &filename, int MethodOption, CoverageParser &Parser);
  // Override the method that gets called for each parsed top-level declaration.
  bool HandleTopLevelDecl(clang::DeclGroupRef DR) override;
  // Create visistor of code coverage tools
  virtual EMIASTVisitor *CreateVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename) = 0;

 protected:
  EMIASTVisitor *Visitor;
  clang::ASTContext &Context;
  std::string filename;
  int MethodOption;
  CoverageParser &Parser;
};

class LLVMCovConsumer : public EMIASTConsumer {
 public:
  LLVMCovConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename, int MethodOption, CoverageParser &Parser);
  EMIASTVisitor *CreateVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename);
};

class GCovConsumer : public EMIASTConsumer {
 public:
  GCovConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename, int MethodOption, CoverageParser &Parser);
  EMIASTVisitor *CreateVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename);
};