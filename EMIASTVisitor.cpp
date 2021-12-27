#include <iostream>

#include "EMIASTVisitor.h"

CovParser *EMIASTVisitor::GCOV = new CovParser(":", 1, 2);
CovParser *EMIASTVisitor::LLVM_COV = new CovParser("\\|", 2, 1);

EMIASTVisitor::EMIASTVisitor(clang::Rewriter &R, clang::ASTContext &Context, std::string filename) : TheRewriter(R), Context(Context)
{
    // std::cout << "filename is " << filename << std::endl;
    // std::map<int, int> *gcovMap = GCOV->parse(filename + ".gcov");
    // for (const auto &e : *gcovMap)
    // {
    //     std::cout << e.first << " = " << e.second << ";";
    // }
    // std::cout << std::endl;

    // std::map<int, int> *llvmCovMap = LLVM_COV->parse(filename + ".llvm-cov");
    // for (const auto &e : *llvmCovMap)
    // {
    //     std::cout << e.first << " = " << e.second << ";";
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
    // Only care about If statements.
    // if (clang::isa<clang::IfStmt>(s))
    // {
    //     clang::IfStmt *IfStatement = clang::cast<clang::IfStmt>(s);

    //     llvm::outs() << "Line number of statement: " << getLineNumber(s) << "\n";

    //     clang::Stmt *Then = IfStatement->getThen();
    //     TheRewriter.InsertText(Then->getBeginLoc(), "// the 'if' part\n", true,
    //                            true);
    //     clang::Stmt *Else = IfStatement->getElse();
    //     if (Else)
    //         TheRewriter.InsertText(Else->getBeginLoc(), "// the 'else' part\n",
    //                                true, true);
    // }

    return true;
}

bool EMIASTVisitor::VisitFunctionDecl(clang::FunctionDecl *f)
{
    // Only function definitions (with bodies), not declarations.
    // if (f->hasBody())
    // {
    //     clang::Stmt *FuncBody = f->getBody();

    //     // Type name as string
    //     clang::QualType QT = f->getReturnType();
    //     std::string TypeStr = QT.getAsString();

    //     // Function name
    //     clang::DeclarationName DeclName = f->getNameInfo().getName();
    //     std::string FuncName = DeclName.getAsString();

    //     // Add comment before
    //     std::stringstream SSBefore;
    //     SSBefore << "// Begin function " << FuncName << " returning " << TypeStr
    //              << "\n";
    //     clang::SourceLocation ST = f->getSourceRange().getBegin();
    //     TheRewriter.InsertText(ST, SSBefore.str(), true, true);

    //     // And after
    //     std::stringstream SSAfter;
    //     SSAfter << "\n// End function " << FuncName;
    //     ST = FuncBody->getEndLoc().getLocWithOffset(1);
    //     TheRewriter.InsertText(ST, SSAfter.str(), true, true);
    // }

    return true;
}