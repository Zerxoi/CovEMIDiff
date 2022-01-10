#include "DiffASTVisitor.h"

DiffASTVisitor::DiffASTVisitor(clang::ASTContext *Context, const std::vector<int> &Lines)
    : Context(Context), Lines(Lines){};

bool isParentLabelStmt(const clang::Stmt *s, clang::ASTContext *Context)
{
    auto parents = Context->getParents(*s);
    while (!parents.empty() && !clang::isa<clang::FunctionDecl>(parents.begin()->get<clang::Decl>()))
    {
        const clang::Stmt *parentStmt = parents.begin()->get<clang::Stmt>();
        if (clang::isa<clang::LabelStmt>(*s))
        {
            return true;
        }
        s = parentStmt;
        parents = Context->getParents(*s);
    }
    return false;
}

bool DiffASTVisitor::VisitStmt(clang::Stmt *s)
{
    if (Index < Lines.size() && Lines[Index] == Context->getSourceManager().getSpellingLineNumber(s->getBeginLoc()))
    {
        if (clang::isa<clang::LabelStmt>(s) && isParentLabelStmt(s, Context))
        {
            llvm::outs() << Lines[Index] << ":"
                         << "Unmarked Label"
                         << "\n";
        }
        Index++;
    }
    return true;
}