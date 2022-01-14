#pragma once

#include "clang/AST/Stmt.h"
#include "clang/AST/ASTContext.h"
namespace util
{
    const clang::Stmt *getSiblingStmt(const clang::Stmt *s, int n, clang::ASTContext *Context);

    bool isAncestorRelation(const clang::Stmt *descendant, const clang::Stmt *ancestor);

    std::string idToString(int id);
}
