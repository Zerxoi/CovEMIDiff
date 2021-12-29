#include "EMIASTConsumer.h"
#include <iostream>

EMIASTConsumer::EMIASTConsumer(clang::ASTContext &Context, std::string &filename)
    : Context(Context), filename(filename) {}

bool EMIASTConsumer::HandleTopLevelDecl(clang::DeclGroupRef DR)
{
    clang::SourceManager &srcMgr = Context.getSourceManager();
    for (auto b = DR.begin(), e = DR.end(); b != e; ++b)
    {
        // only manipulate the specific file
        if (!srcMgr.getFilename((*b)->getLocation()).equals(filename))
        {
            continue;
        }
        // Traverse the declaration using our AST visitor.
        Visitor->TraverseDecl(*b);
    }
    return true;
}

GCovConsumer::GCovConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename) : EMIASTConsumer(Context, filename)
{
    Visitor = CreateVisitor(R, Context, filename);
}

EMIASTVisitor *GCovConsumer::CreateVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename)
{
    CoverageParser *parser = new CoverageParser("^\\s*#####:\\s*(\\d+):.*$");
    std::string extension = ".gcov";
    return new EMIASTVisitor(R, Context, filename, parser, extension);
}

LLVMCovConsumer::LLVMCovConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename) : EMIASTConsumer(Context, filename)
{
    Visitor = CreateVisitor(R, Context, filename);
};

EMIASTVisitor *LLVMCovConsumer::CreateVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename)
{
    CoverageParser *parser = new CoverageParser("^\\s*(\\d+)\\|\\s*0\\|.*$");
    std::string extension = ".llvm-cov";
    return new EMIASTVisitor(R, Context, filename, parser, extension);
}