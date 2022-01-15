#pragma once

#include "clang/AST/Stmt.h"
#include "clang/AST/ASTContext.h"
namespace util
{
    const clang::Stmt *getSiblingStmt(const clang::Stmt *s, int n, clang::ASTContext *Context);

    bool bfs(const clang::Stmt *root, std::function<bool(const clang::Stmt *)> find);

    std::string idToString(int id);
}
