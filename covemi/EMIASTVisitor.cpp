#include "EMIConst.h"
#include "EMIASTVisitor.h"

EMIASTVisitor::EMIASTVisitor(clang::Rewriter &r, clang::ASTContext &context, std::string filename, CoverageParser &parser, std::string extension)
    : Parser(parser), Extension(extension), TheRewriter(r), Context(context)
{
    Unexecuted = Parser.parse(filename + Extension);
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

PreASTVisitor::PreASTVisitor(clang::Rewriter &r, clang::ASTContext &context, std::string filename, CoverageParser &parser, std::string extension)
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

PostASTVisitor::PostASTVisitor(clang::Rewriter &r, clang::ASTContext &context, std::string filename, CoverageParser &parser, std::string extension)
    : EMIASTVisitor(r, context, filename, parser, extension)
{
}

bool PostASTVisitor::shouldTraversePostOrder() const
{
    return true;
}
// Determine whether a statement can be deleted
// 5 types of statements that can be deleted:
//      1. Statement in IfStmt that is not conditions and is not CompoundStmt
//      2. Statement in ForStmt that is not conditions and is not CompoundStmt
//      3. Statement in WhileStmt that is not conditions and is not CompoundStmt
//      4. Statement in Label that  is not CompoundStmt
//      5. Statement in CompoundStmt that is not CompoundStmt
bool isDeletableStmt(const clang::Stmt *stmt, const clang::Stmt *parent)
{
    if (stmt == nullptr || parent == nullptr || clang::isa<clang::CompoundStmt>(stmt))
    {
        return false;
    }
    // stmt->dumpColor();
    if (clang::isa<clang::IfStmt>(parent))
    {
        auto ifParent = clang::cast<clang::IfStmt>(parent);
        if (stmt == ifParent->getThen() || stmt == ifParent->getElse())
        {
            return true;
        }
        return false;
    }
    if (clang::isa<clang::ForStmt>(parent))
    {
        auto forParent = clang::cast<clang::ForStmt>(parent);
        if (stmt == forParent->getBody())
        {
            return true;
        }
        return false;
    }
    if (clang::isa<clang::WhileStmt>(parent))
    {
        auto whileParent = clang::cast<clang::WhileStmt>(parent);
        if (stmt == whileParent->getBody())
        {
            return true;
        }
        return false;
    }
    if (clang::isa<clang::CompoundStmt>(parent) || clang::isa<clang::LabelStmt>(parent))
    {
        return true;
    }
    return false;
}

// Determine whether a statement can be deleted according to its child statement, the statement
// can be deleted if there is no executed statement in the child statement
bool shouldDeleteStmt(clang::Stmt *stmt, clang::ASTContext &Context, const std::set<int> *Unexecuted, const std::string &Extension)
{
    for (auto child : stmt->children())
    {
        // child->dumpColor();
        // If the child statement is a deleteable statement, it is judged directly according to whether
        // it is executed or not.
        if (isDeletableStmt(child, stmt))
        {
            int line = Context.getSourceManager().getSpellingLineNumber(child->getBeginLoc());
            if (!Unexecuted->count(line) && (Extension.compare(extension::gcov) || !clang::isa<clang::DeclStmt>(child)))
            {
                return false;
            }
        }
        // If the child statement is CompoundStmt, it should be deleted as long as there are no executed statements
        // in the CompoundStmt's child statement
        if (child != nullptr && clang::isa<clang::CompoundStmt>(child))
        {
            for (auto grandchild : child->children())
            {
                int line = Context.getSourceManager().getSpellingLineNumber(grandchild->getBeginLoc());
                // In addition to unexecuted statements should be deleted, DeclStatements that are not marked
                // in the .gcov file should also be deleted
                // !(Unexecuted->count(line) || (Extension.compare(extension::gcov) == 0 && clang::isa<clang::DeclStmt>(grandchild))
                if (!Unexecuted->count(line) && (Extension.compare(extension::gcov) || !clang::isa<clang::DeclStmt>(grandchild)))
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool PostASTVisitor::VisitStmt(clang::Stmt *s)
{
    auto scope = Context.getTraversalScope();
    int beginLine = Context.getSourceManager().getSpellingLineNumber(s->getBeginLoc());
    if (Unexecuted->count(beginLine) && !clang::isa<clang::CompoundStmt>(s))
    {
        auto parents = Context.getParents(*s);
        if (!parents.empty())
        {
            auto parentStmt = parents.begin()->get<clang::Stmt>();
            // Because the expression is a statement subclass, if the unexecuted statement has a
            // child statement executed, then the statement should not be deleted, but the expression
            // in the statement may still be deleted. Therefore, this problem can be avoided if
            // only the direct child statement of the compound statement can be deleted
            if (isDeletableStmt(s, parentStmt) && shouldDeleteStmt(s, Context, Unexecuted, Extension))
            {
                // s->dumpColor();
                // llvm::outs() << "BeginLine: " << beginLine << "\n";
                TheRewriter.RemoveText(s->getSourceRange());
            }
        }
    }
    return true;
}