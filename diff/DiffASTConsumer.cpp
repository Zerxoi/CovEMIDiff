#include "DiffASTConsumer.h"

DiffASTConsumer::DiffASTConsumer(const clang::ASTContext *Context, const std::string &Filename, const std::vector<int> &Lines)
    : Context(Context), Visitor(Context, Lines), Filename(Filename){};

bool DiffASTConsumer::HandleTopLevelDecl(clang::DeclGroupRef D)
{
    const clang::SourceManager &srcMgr = Context->getSourceManager();
    for (auto b = D.begin(), e = D.end(); b != e; b++)
    {
        std::string filename = srcMgr.getFilename((*b)->getLocation());
        if (srcMgr.getFilename((*b)->getLocation()).equals(Filename))
        {
            Visitor.TraverseDecl(*b);
        }
    }
    return true;
}