#include "EMIASTConsumer.h"

#include "EMIConst.h"

EMIASTConsumer::EMIASTConsumer(clang::ASTContext &Context, std::string &filename, const std::string &CoverageToolVersion, int MethodOption)
    : Context(Context), filename(filename), CoverageToolVersion(CoverageToolVersion), MethodOption(MethodOption) {}

bool EMIASTConsumer::HandleTopLevelDecl(clang::DeclGroupRef DR) {
  clang::SourceManager &srcMgr = Context.getSourceManager();
  for (auto b = DR.begin(), e = DR.end(); b != e; ++b) {
    // only manipulate the specific file
    if (!srcMgr.getFilename((*b)->getLocation()).equals(filename)) {
      continue;
    }
    // Set the ASTContext parsing scope manually so that getParents() can successfully get the parent statement
    Context.setTraversalScope({*b});
    // Traverse the declaration using our AST visitor.
    Visitor->TraverseDecl(*b);
  }
  return true;
}

GCovConsumer::GCovConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename, const std::string &CoverageToolVersion, int MethodOption)
    : EMIASTConsumer(Context, filename, CoverageToolVersion, MethodOption) {
  Visitor = CreateVisitor(R, Context, filename);
}

EMIASTVisitor *GCovConsumer::CreateVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename) {
  CoverageParser Parser = CoverageParser(parser::gcov::executed, parser::gcov::unexecuted, parser::gcov::isCountBeforeLineNum);
  if (MethodOption == 1) {
    return new PostASTVisitor(R, Context, filename, Parser, CoverageToolVersion, extension::gcov);
  }
  return new PreASTVisitor(R, Context, filename, Parser, CoverageToolVersion, extension::gcov);
}

LLVMCovConsumer::LLVMCovConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename, const std::string &CoverageToolVersion, int MethodOption)
    : EMIASTConsumer(Context, filename, CoverageToolVersion, MethodOption) {
  Visitor = CreateVisitor(R, Context, filename);
};

EMIASTVisitor *LLVMCovConsumer::CreateVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename) {
  CoverageParser Parser = CoverageParser(parser::llvmcov::executed, parser::llvmcov::unexecuted, parser::llvmcov::isCountBeforeLineNum);
  if (MethodOption == 1) {
    return new PostASTVisitor(R, Context, filename, Parser, CoverageToolVersion, extension::llvmcov);
  }
  return new PreASTVisitor(R, Context, filename, Parser, CoverageToolVersion, extension::llvmcov);
}