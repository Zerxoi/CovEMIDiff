#include <iostream>

#include "EMIASTVisitor.h"

EMIASTVisitor::EMIASTVisitor(clang::Rewriter &r, clang::ASTContext &context, std::string filename, CoverageParser *parser, std::string extension)
    : TheRewriter(r), Context(context), Parser(parser), Extension(extension)
{
    Unexecuted = Parser->parse(filename + Extension);
    for (const auto &line : *Unexecuted)
    {
        std::cout << line << "; ";
    }
    std::cout << std::endl;
}

int EMIASTVisitor::getLineNumber(const clang::Stmt *stmt)
{
    clang::FullSourceLoc FullLocation = Context.getFullLoc(stmt->getBeginLoc());
    bool invalid = FullLocation.isInvalid();
    return FullLocation.getSpellingLineNumber(&invalid);
}

bool EMIASTVisitor::VisitStmt(clang::Stmt *s)
{
    // Only care about If statements.
    if (clang::isa<clang::IfStmt>(s))
    {
        clang::IfStmt *IfStatement = clang::cast<clang::IfStmt>(s);

        llvm::outs() << "Line number of statement: " << getLineNumber(s) << "\n";

        clang::Stmt *Then = IfStatement->getThen();
        TheRewriter.InsertText(Then->getBeginLoc(), "// the 'if' part\n", true,
                               true);
        clang::Stmt *Else = IfStatement->getElse();
        if (Else)
            TheRewriter.InsertText(Else->getBeginLoc(), "// the 'else' part\n",
                                   true, true);
    }

    return true;
}

bool EMIASTVisitor::VisitFunctionDecl(clang::FunctionDecl *f)
{
    // Only function definitions (with bodies), not declarations.
    if (f->hasBody())
    {
        clang::Stmt *FuncBody = f->getBody();

        // Type name as string
        clang::QualType QT = f->getReturnType();
        std::string TypeStr = QT.getAsString();

        // Function name
        clang::DeclarationName DeclName = f->getNameInfo().getName();
        std::string FuncName = DeclName.getAsString();

        // Add comment before
        std::stringstream SSBefore;
        SSBefore << "// Begin function " << FuncName << " returning " << TypeStr
                 << "\n";
        clang::SourceLocation ST = f->getSourceRange().getBegin();
        TheRewriter.InsertText(ST, SSBefore.str(), true, true);

        // And after
        std::stringstream SSAfter;
        SSAfter << "\n// End function " << FuncName;
        ST = FuncBody->getEndLoc().getLocWithOffset(1);
        TheRewriter.InsertText(ST, SSAfter.str(), true, true);
    }

    return true;
}