#include "EMIASTConsumer.h"

EMIASTConsumer::EMIASTConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename) : Visitor(R, Context, filename) {}

bool EMIASTConsumer::HandleTopLevelDecl(clang::DeclGroupRef DR)
{
    for (auto b = DR.begin(), e = DR.end(); b != e; ++b)
    {
        // Traverse the declaration using our AST visitor.
        Visitor.TraverseDecl(*b);
        // (*b)->dump();
    }
    return true;
}
