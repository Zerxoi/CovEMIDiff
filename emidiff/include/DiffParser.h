#pragma once

#include "DiffEnum.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Stmt.h"

// Parse the AST tree of inconsistent lines in the file and determine whether it is caused by a specific Diff Bug
class DiffParser {
 public:
  DiffParser(const enum coverageTool CoverageTool, const enum coverageTool FileType, const std::string &Description);
  virtual bool parse(const clang::Stmt *s, clang::ASTContext *Context) = 0;
  const enum coverageTool &getCoverageTool() const;
  const enum coverageTool &getFileType() const;
  const std::string &getDescription() const;
  const int getCount() const;

 private:
  const enum coverageTool CoverageTool;  // Diff bug is at that coverage tool's ID
  const enum coverageTool FileType;      // In which coverage tool's EMI file type can the Diff bug be found
  const std::string &Description;

 protected:
  int Count = 0;
};

class IfOptimizeDiffParser : public DiffParser {
 public:
  explicit IfOptimizeDiffParser();
  virtual bool parse(const clang::Stmt *s, clang::ASTContext *Context) override;
  // Wraps the clang::Expr's isEvaluatable method
  bool isEvaluatable(const clang::Expr *expr, const clang::ASTContext &Context);

 private:
  const clang::Stmt *IfOptimizeStmt;
};

class UnmarkedLabelDiffParser : public DiffParser {
 public:
  explicit UnmarkedLabelDiffParser();
  virtual bool parse(const clang::Stmt *s, clang::ASTContext *Context) override;

 private:
  const clang::Stmt *UnmarkedLabelStmt;
};

class ConstArrayInitializationDiffParser : public DiffParser {
 public:
  explicit ConstArrayInitializationDiffParser();
  virtual bool parse(const clang::Stmt *s, clang::ASTContext *Context) override;
};

class JumpBlockDiffParser : public DiffParser {
 public:
  explicit JumpBlockDiffParser();
  virtual bool parse(const clang::Stmt *s, clang::ASTContext *Context) override;

 private:
  const clang::Stmt *PreJumpBlockStmt;
};

const std::vector<DiffParser *> *createDiffParserVector();