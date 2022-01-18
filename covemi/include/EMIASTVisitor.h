#pragma once
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/RecursiveASTVisitor.h"

#include "CoverageParser.h"

// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class EMIASTVisitor : public clang::RecursiveASTVisitor<EMIASTVisitor>
{
public:
    EMIASTVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename, CoverageParser &parser, std::string extension);

    // Get line number of the statement
    int getLineNumber(const clang::Stmt *stmt);
    virtual bool VisitStmt(clang::Stmt *s);
    virtual bool shouldTraversePostOrder() const;

protected:
    CoverageParser &Parser;
    std::string Extension;
    std::set<int> *Unexecuted;
    clang::Rewriter &TheRewriter;
    clang::ASTContext &Context;
};

// Visit statement in preorder
// If the command is not executed, regardless of whether the clause is executed,
// the statement will be pruned, in order to expose more inconsistencies in results
class PreASTVisitor : public EMIASTVisitor
{
public:
    PreASTVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename, CoverageParser &parser, std::string extension);
    bool VisitStmt(clang::Stmt *s);
};

// Visit statement in postorder
// If the statement is not executed, but the sub-statement of the statement is executed,
// the statement will not be deleted, so that the granularity of pruning will be smaller
// and it is easier to find inconsistent coverage
class PostASTVisitor : public EMIASTVisitor
{
public:
    PostASTVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename, CoverageParser &parser, std::string extension);
    bool shouldTraversePostOrder() const;
    bool VisitStmt(clang::Stmt *s);
};
