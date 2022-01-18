#include "Const.h"
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
        // Set the ASTContext parsing scope manually so that getParents() can successfully get the parent statement
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
    CoverageParser *parser = new CoverageParser(parser::gcov::executed, parser::gcov::unexecuted, parser::gcov::isCountBeforeLineNum);
    if (MethodOption == 1)
    {
        return new PostASTVisitor(R, Context, filename, parser, extension::gcov);
    }
    return new PreASTVisitor(R, Context, filename, parser, extension::gcov);
}

LLVMCovConsumer::LLVMCovConsumer(clang::Rewriter &R, clang::ASTContext &Context, std::string filename, int MethodOption) : EMIASTConsumer(Context, filename, MethodOption)
{
    Visitor = CreateVisitor(R, Context, filename);
};

EMIASTVisitor *LLVMCovConsumer::CreateVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename)
{
    CoverageParser *parser = new CoverageParser(parser::llvmcov::executed, parser::llvmcov::unexecuted, parser::llvmcov::isCountBeforeLineNum);
    if (MethodOption == 1)
    {
        return new PostASTVisitor(R, Context, filename, parser, extension::llvmcov);
    }
    return new PreASTVisitor(R, Context, filename, parser, extension::llvmcov);
}