#include <iostream>

#include "EMIASTVisitor.h"

EMIASTVisitor::EMIASTVisitor(clang::Rewriter &r, clang::ASTContext &context, std::string filename, CoverageParser *parser, std::string extension)
    : Parser(parser), Extension(extension), TheRewriter(r), Context(context)
{
    Unexecuted = Parser->parse(filename + Extension);
    // Log unexecuted lines
    // for (const auto &line : *Unexecuted)
    // {
    //     std::cout << line << "; ";
    // }
    // std::cout << std::endl;
}

int EMIASTVisitor::getLineNumber(const clang::Stmt *stmt)
{
    clang::FullSourceLoc FullLocation = Context.getFullLoc(stmt->getBeginLoc());
    bool invalid = FullLocation.isInvalid();
    return FullLocation.getSpellingLineNumber(&invalid);
}

bool EMIASTVisitor::VisitStmt(clang::Stmt *s)
{
    int lineNumber = getLineNumber(s);
    if (Unexecuted->count(lineNumber))
    {
        Unexecuted->erase(lineNumber);

        // Log debug information
        // std::cout << "LineNumber: " << lineNumber << std::endl;
        // s->dump();

        // Prune unexecuted statement
        // Operations on compound statements will cause repeated operations on
        // statements. To avoid this case, compound statements are not be pruned
        if (!clang::isa<clang::CompoundStmt>(s))
        {
            TheRewriter.RemoveText(s->getSourceRange());
        }
    }
    return true;
}