#include "DiffASTConsumer.h"

DiffASTConsumer::DiffASTConsumer(clang::ASTContext *Context, const std::vector<int> &Lines, const int CoverageToolId, const std::string &Filename, const std::vector<DiffParser *> *DiffParserVector)
    : Context(Context), Visitor(Context, Lines, CoverageToolId, DiffParserVector), Filename(Filename){};

bool DiffASTConsumer::HandleTopLevelDecl(clang::DeclGroupRef D)
{
    const clang::SourceManager &srcMgr = Context->getSourceManager();
    for (auto b = D.begin(), e = D.end(); b != e; b++)
    {
        if (srcMgr.getFilename((*b)->getLocation()).equals(Filename))
        {
            Context->setTraversalScope({*b});
            Visitor.TraverseDecl(*b);
        }
    }
    return true;
}