#include "EMIASTVisitor.h"

EMIASTVisitor::EMIASTVisitor(clang::Rewriter &r, clang::ASTContext &context, std::string filename, CoverageParser *parser, std::string extension)
    : Parser(parser), Extension(extension), TheRewriter(r), Context(context)
{
    Unexecuted = Parser->parse(filename + Extension);
    // Log unexecuted lines
    // for (const auto &line : *Unexecuted)
    // {
    //     llvm::outs() << line << "; ";
    // }
    // llvm::outs() << "\n";
}

int EMIASTVisitor::getLineNumber(const clang::Stmt *stmt)
{
    clang::FullSourceLoc FullLocation = Context.getFullLoc(stmt->getBeginLoc());
    bool invalid = FullLocation.isInvalid();
    return FullLocation.getSpellingLineNumber(&invalid);
}

bool EMIASTVisitor::VisitStmt(clang::Stmt *s)
{
    clang::SourceRange range = s->getSourceRange();
    clang::SourceManager &srcMgr = Context.getSourceManager();
    int beginLine = srcMgr.getSpellingLineNumber(range.getBegin());
    // Prune unexecuted statement
    // Only unexecuted Stmt can be removed, and operations on compound statements
    // will cause repeated operations on statements. To avoid this case, compound
    // statements are not be pruned
    if (Unexecuted->count(beginLine) && !clang::isa<clang::CompoundStmt>(s))
    {
        int endLine = srcMgr.getSpellingLineNumber(range.getEnd());
        // Stmt is traversed in DFS order. If the parent node is deleted,
        // it can still traverse to the child Stmt. If the child Stmt is
        // also deleted, a segfault will be thrown.
        // In order to solve this problem, the deletion of the unexecuted
        // parent Stmt deletes all the unexecuted child Stmts. On the one
        // hand, it can avoid the repeated deletion of the child Stmt, and
        // on the other hand, it will expose the problems in the code
        // coverage tool as much as possible.
        for (int i = beginLine; i <= endLine; i++)
        {
            Unexecuted->erase(i);
        }

        // Log debug information
        // s->dump();
        // llvm::outs() << "Statement Begin Line: " << beginLine << "\n";
        // llvm::outs() << "Statement End Line: " << endLine << "\n";

        TheRewriter.RemoveText(s->getSourceRange());
    }
    return true;
}