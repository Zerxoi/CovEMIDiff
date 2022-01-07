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
    return true;
}

bool EMIASTVisitor::shouldTraversePostOrder() const
{
    return false;
}

PreASTVisitor::PreASTVisitor(clang::Rewriter &r, clang::ASTContext &context, std::string filename, CoverageParser *parser, std::string extension)
    : EMIASTVisitor(r, context, filename, parser, extension)
{
}

bool PreASTVisitor::VisitStmt(clang::Stmt *s)
{
    clang::SourceManager &srcMgr = Context.getSourceManager();
    int beginLine = srcMgr.getSpellingLineNumber(s->getBeginLoc());
    // Prune unexecuted statement
    // Only unexecuted Stmt can be removed, and operations on compound statements
    // will cause repeated operations on statements. To avoid this case, compound
    // statements are not be pruned
    if (Unexecuted->count(beginLine) && !clang::isa<clang::CompoundStmt>(s))
    {
        int endLine = srcMgr.getSpellingLineNumber(s->getEndLoc());
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

        TheRewriter.RemoveText(s->getSourceRange());
    }
    return true;
}

PostASTVisitor::PostASTVisitor(clang::Rewriter &r, clang::ASTContext &context, std::string filename, CoverageParser *parser, std::string extension)
    : EMIASTVisitor(r, context, filename, parser, extension)
{
}

bool PostASTVisitor::shouldTraversePostOrder() const
{
    return true;
}

bool PostASTVisitor::VisitStmt(clang::Stmt *s)
{
    clang::SourceRange range = s->getSourceRange();
    clang::SourceManager &srcMgr = Context.getSourceManager();
    int beginLine = srcMgr.getSpellingLineNumber(range.getBegin());
    if (Unexecuted->count(beginLine) && !clang::isa<clang::CompoundStmt>(s))
    {
        auto parents = Context.getParents(*s);
        if (!parents.empty())
        {
            auto parentStmt = parents.begin()->get<clang::Stmt>();
            if (parentStmt != nullptr && clang::isa<clang::CompoundStmt>(parentStmt))
            {
                bool shouldDelete = true;
                for (auto &child : s->children())
                {
                    if (child != nullptr && clang::isa<clang::CompoundStmt>(child))
                    {
                        for (auto &grandchild : child->children())
                        {
                            int line = Context.getSourceManager().getSpellingLineNumber(grandchild->getBeginLoc());
                            if (!Unexecuted->count(line))
                            {
                                shouldDelete = false;
                                break;
                            }
                        }
                    }
                }
                if (shouldDelete)
                {
                    // s->dumpColor();
                    // llvm::outs() << "BeginLine: " << beginLine << "\n";
                    TheRewriter.RemoveText(s->getSourceRange());
                }
            }
        }
    }
    return true;
}