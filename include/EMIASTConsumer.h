#include "clang/AST/ASTConsumer.h"

#include "EMIASTVisitor.h"

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class EMIASTConsumer : public clang::ASTConsumer
{
private:
    EMIASTVisitor Visitor;

public:
    EMIASTConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename);

    // Override the method that gets called for each parsed top-level
    // declaration.
    bool HandleTopLevelDecl(clang::DeclGroupRef DR) override;
};