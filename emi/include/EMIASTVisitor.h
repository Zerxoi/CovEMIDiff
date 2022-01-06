#pragma once
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include "CoverageParser.h"

// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class EMIASTVisitor : public clang::RecursiveASTVisitor<EMIASTVisitor>
{
protected:
    CoverageParser *Parser;
    std::string Extension;

    std::set<int> *Unexecuted;
    clang::Rewriter &TheRewriter;
    clang::ASTContext &Context;

public:
    EMIASTVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename, CoverageParser *parser, std::string extension);

    // Get line number of the statement
    int getLineNumber(const clang::Stmt *stmt);

    virtual bool VisitStmt(clang::Stmt *s);
    virtual bool shouldTraversePostOrder() const;
};

class PreASTVisitor : public EMIASTVisitor
{
public:
    PreASTVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename, CoverageParser *parser, std::string extension);

    bool VisitStmt(clang::Stmt *s);
};

class PostASTVisitor : public EMIASTVisitor
{
public:
    PostASTVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename, CoverageParser *parser, std::string extension);

    bool shouldTraversePostOrder() const;
    bool VisitStmt(clang::Stmt *s);
};
