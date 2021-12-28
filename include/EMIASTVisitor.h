#pragma once
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include "CoverageParser.h"

// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class EMIASTVisitor : public clang::RecursiveASTVisitor<EMIASTVisitor>
{
private:
    CoverageParser *Parser;
    std::string Extension;

    std::set<int> *Unexecuted;
    clang::Rewriter &TheRewriter;
    clang::ASTContext &Context;

public:
    EMIASTVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename, CoverageParser *parser, std::string extension);

    int getLineNumber(const clang::Stmt *stmt);

    bool VisitStmt(clang::Stmt *s);

    bool VisitFunctionDecl(clang::FunctionDecl *f);
};