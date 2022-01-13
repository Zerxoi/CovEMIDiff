#include "Util.h"

const clang::Stmt *getSiblingStmt(const clang::Stmt *s, int n, clang::ASTContext *Context)
{
    if (n == 0)
    {
        return s;
    }
    auto parents = Context->getParents(*s);
    if (parents.empty())
    {
        return nullptr;
    }
    const clang::Stmt *parent = parents.begin()->get<clang::Stmt>();
    auto current = parent->child_begin();
    auto target = parent->child_begin();
    bool reverse = false;
    if (n < 0)
    {
        reverse = true;
        n = -n;
    }
    while (current != parent->child_end() && n--)
    {
        current++;
    }
    if (n > 0)
    {
        return nullptr;
    }
    if (reverse)
    {
        auto tmp = current;
        current = target;
        target = tmp;
    }
    while (target != parent->child_end() && current != parent->child_end() && *target != s)
    {
        current++;
        target++;
    }
    if (*target == s)
    {
        return *current;
    }
    return nullptr;
}