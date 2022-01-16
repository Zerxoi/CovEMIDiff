#pragma once

#include "clang/AST/Stmt.h"
#include "clang/AST/ASTContext.h"

#include "Const.h"
// Diff util namespace
namespace util
{
    // Return the 'n'th sibling statement of statement 's' in the increasing direction of the number of lines
    // If n is 0 return statement s itself; If n is -1 return the previous statement of statement s
    // Others are the same
    const clang::Stmt *getSiblingStmt(const clang::Stmt *s, int n, clang::ASTContext *Context);

    // Finds whether there is a statement satisfying the specified condition in the AST tree by breadth-first traversal
    bool bfs(const clang::Stmt *root, std::function<bool(const clang::Stmt *)> find);

    std::string str(enum coverageTool ct);
}
