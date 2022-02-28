#include "DiffParser.h"

#include "DiffConst.h"
#include "DiffUtil.h"
#include "clang/AST/Expr.h"

DiffParser::DiffParser(const enum coverageTool CoverageTool, const enum coverageTool FileType, const std::string &Description) : CoverageTool(CoverageTool), FileType(FileType), Description(Description) {}

const enum coverageTool &DiffParser::getCoverageTool() const { return CoverageTool; }

const enum coverageTool &DiffParser::getFileType() const { return FileType; }

const std::string &DiffParser::getDescription() const { return Description; }

const int DiffParser::getCount() const { return Count; }

IfOptimizeDiffParser::IfOptimizeDiffParser() : IfOptimizeStmt(nullptr), DiffParser(parser::ifOptimize::coverageTool, parser::ifOptimize::fileType, reason::description::ifOptimize) {}

// Check if an expression is evaluatable
// clang::Expr.isEvaluatable method cannot evaluate comma expressions
bool IfOptimizeDiffParser::isEvaluatable(const clang::Expr *expr, const clang::ASTContext &Context) {
  if (clang::isa<clang::OpaqueValueExpr>(expr)) {
    return true;
  }
  if (auto binOpr = clang::dyn_cast<clang::BinaryOperator>(expr)) {
    if (binOpr->isCommaOp()) {
      return isEvaluatable(binOpr->getRHS(), Context);
    }
  }
  if (auto parenOpr = clang::dyn_cast<clang::ParenExpr>(expr)) {
    return isEvaluatable(parenOpr->getSubExpr(), Context);
  }
  return expr->isEvaluatable(Context);
}

bool IfOptimizeDiffParser::parse(const clang::Stmt *s, clang::ASTContext *Context) {
  if (auto ifStmt = clang::dyn_cast<clang::IfStmt>(s)) {
    if (isEvaluatable(ifStmt->getCond(), *Context)) {
      // Prevent If Optimize from nesting normal if statement
      if (!util::bfs(IfOptimizeStmt, [s](auto child) { return child == s; })) {
        // If the If statment is not a child statement of If Optimize, the if statement is a new If Optimize
        IfOptimizeStmt = s;
        Count++;
      }
      return true;
    }
  }
  // Child statements are also If Optimize
  if (util::bfs(IfOptimizeStmt, [s](auto child) { return child == s; })) {
    return true;
  }
  return false;
}

UnmarkedLabelDiffParser::UnmarkedLabelDiffParser() : UnmarkedLabelStmt(nullptr), DiffParser(parser::unmarkedLabel::coverageTool, parser::unmarkedLabel::fileType, reason::description::unmarkedLabel){};

bool UnmarkedLabelDiffParser::parse(const clang::Stmt *s, clang::ASTContext *Context) {
  if (clang::isa<clang::LabelStmt>(s)) {
    // Prevent Unmarked Label from nesting normal Label
    if (!util::bfs(UnmarkedLabelStmt, [s](auto child) { return child == s; })) {
      // If Label is not a child statement of Unmarked Label, the Label is a new Unmarked Label
      UnmarkedLabelStmt = s;
      Count++;
    }
    return true;
  }
  // Child statements are also Unmarked Label
  if (util::bfs(UnmarkedLabelStmt, [s](auto child) { return child == s; })) {
    return true;
  }
  return false;
}

ConstArrayInitializationDiffParser::ConstArrayInitializationDiffParser() : DiffParser(parser::constArrayInitialization::coverageTool, parser::constArrayInitialization::fileType, reason::description::constArrayInitialization){};

bool ConstArrayInitializationDiffParser::parse(const clang::Stmt *s, clang::ASTContext *Context) {
  // There is an initialization of a const array in the declaration statement
  if (auto declStmt = clang::dyn_cast<clang::DeclStmt>(s)) {
    for (auto decl : declStmt->decls()) {
      if (auto *varDecl = clang::dyn_cast<clang::VarDecl>(decl)) {
        if (varDecl->hasInit() && clang::isa<clang::ConstantArrayType>(varDecl->getType()) && varDecl->getType().isConstQualified()) {
          Count++;
          return true;
        }
      }
    }
  }
  return false;
}

JumpBlockDiffParser::JumpBlockDiffParser() : PreJumpBlockStmt(nullptr), DiffParser(parser::jumpBlock::coverageTool, parser::jumpBlock::fileType, reason::description::jumpBlock) {}

bool JumpBlockDiffParser::parse(const clang::Stmt *s, clang::ASTContext *Context) {
  auto preStmt = util::getSiblingStmt(s, -1, Context);
  // If there is a previous JumpBlock statement in the clause of the previous sibling statement,
  // it means that this statement is a JumpBlock statement
  if (PreJumpBlockStmt != nullptr && util::bfs(preStmt, [this](const clang::Stmt *child) { return child == PreJumpBlockStmt; })) {
    PreJumpBlockStmt = s;
    return true;
  }
  // If the previous sibling statement is a clause that does not contain the previous JumpBlock statement
  // but does contain a jump statement, then it may be a new JumpBlock statement
  if (util::bfs(preStmt, [](const clang::Stmt *child) { return child != nullptr && (clang::isa<clang::ReturnStmt>(child) || clang::isa<clang::BreakStmt>(child) || clang::isa<clang::ContinueStmt>(child)); })) {
    Count++;
    PreJumpBlockStmt = s;
    return true;
  }
  return false;
}

const std::vector<DiffParser *> *createDiffParserVector() {
  std::vector<DiffParser *> *vector = new std::vector<DiffParser *>;
  vector->push_back(new IfOptimizeDiffParser);
  vector->push_back(new UnmarkedLabelDiffParser);
  vector->push_back(new ConstArrayInitializationDiffParser);
  vector->push_back(new JumpBlockDiffParser);

  return vector;
}