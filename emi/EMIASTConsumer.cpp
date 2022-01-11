#include "EMIASTConsumer.h"

EMIASTConsumer::EMIASTConsumer(clang::ASTContext &Context, std::string &filename, int MethodOption)
    : Context(Context), filename(filename), MethodOption(MethodOption) {}

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
        // Manually set the ASTContext parsing scope so that getParents() can successfully get the parent statement
        Context.setTraversalScope({*b});
        // Traverse the declaration using our AST visitor.
        Visitor->TraverseDecl(*b);
    }
    return true;
}

GCovConsumer::GCovConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename, int MethodOption) : EMIASTConsumer(Context, filename, MethodOption)
{
    Visitor = CreateVisitor(R, Context, filename);
}

EMIASTVisitor *GCovConsumer::CreateVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename)
{
    CoverageParser *parser = new CoverageParser("^\\s*#####:\\s*(\\d+):.*$");
    std::string extension = ".gcov";
    if (MethodOption == 1)
    {
        return new PostASTVisitor(R, Context, filename, parser, extension);
    }
    return new PreASTVisitor(R, Context, filename, parser, extension);
}

LLVMCovConsumer::LLVMCovConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename, int MethodOption) : EMIASTConsumer(Context, filename, MethodOption)
{
    Visitor = CreateVisitor(R, Context, filename);
};

EMIASTVisitor *LLVMCovConsumer::CreateVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename)
{
    CoverageParser *parser = new CoverageParser("^\\s*(\\d+)\\|\\s*0\\|.*$");
    std::string extension = ".llvm-cov";
    if (MethodOption == 1)
    {
        return new PostASTVisitor(R, Context, filename, parser, extension);
    }
    return new PreASTVisitor(R, Context, filename, parser, extension);
}