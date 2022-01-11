#include "DiffASTVisitor.h"

DiffASTVisitor::DiffASTVisitor(clang::ASTContext *Context, const std::vector<int> &Lines)
    : Context(Context), Lines(Lines){};

bool isUnmarkedLabel(const clang::Stmt *s, clang::ASTContext *Context)
{
    llvm::outs() << Context->getSourceManager().getSpellingLineNumber(s->getBeginLoc()) << "\n";
    if (clang::isa<clang::LabelStmt>(s))
    {
        return true;
    }
    auto parents = Context->getParents(*s);
    while (!parents.empty())
    {
        const clang::Stmt *parentStmt = parents.begin()->get<clang::Stmt>();
        if (clang::isa<clang::LabelStmt>(*parentStmt))
        {
            return true;
        }
        parents = Context->getParents(*parentStmt);
    }
    return false;
}

bool DiffASTVisitor::VisitStmt(clang::Stmt *s)
{
    if (Index < Lines.size() && Lines[Index] == Context->getSourceManager().getSpellingLineNumber(s->getBeginLoc()))
    {
        if (isUnmarkedLabel(s, Context))
        {
            llvm::outs() << Lines[Index] << ":"
                         << "Unmarked Label"
                         << "\n";
        }
        Index++;
    }
    return true;
}