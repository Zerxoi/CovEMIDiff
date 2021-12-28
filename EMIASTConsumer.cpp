#include "EMIASTConsumer.h"

bool EMIASTConsumer::HandleTopLevelDecl(clang::DeclGroupRef DR)
{
    for (auto b = DR.begin(), e = DR.end(); b != e; ++b)
    {
        // Traverse the declaration using our AST visitor.
        Visitor->TraverseDecl(*b);
        // (*b)->dump();
    }
    return true;
}

LLVMCovConsumer::LLVMCovConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename)
{
    Visitor = CreateVisitor(R, Context, filename);
};

EMIASTVisitor *LLVMCovConsumer::CreateVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename)
{
    CoverageParser *parser = new CoverageParser("^\\s*(\\d+)\\|\\s*0\\|.*$");
    std::string extension = ".gcov";
    return new EMIASTVisitor(R, Context, filename, parser, extension);
}

GCovConsumer::GCovConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename)
{
    Visitor = CreateVisitor(R, Context, filename);
};

EMIASTVisitor *GCovConsumer::CreateVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename)
{
    CoverageParser *parser = new CoverageParser("^\\s*#####:\\s*(\\d+):.*$");
    std::string extension = ".llvm-cov";
    return new EMIASTVisitor(R, Context, filename, parser, extension);
}