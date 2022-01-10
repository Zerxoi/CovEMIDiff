#include "DiffASTConsumer.h"

DiffASTConsumer::DiffASTConsumer(clang::ASTContext *Context)
    : Visitor(Context){};

void DiffASTConsumer::HandleTranslationUnit(clang::ASTContext &Context)
{
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
};