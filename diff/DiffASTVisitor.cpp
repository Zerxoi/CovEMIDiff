#include "DiffASTVisitor.h"

DiffASTVisitor::DiffASTVisitor(clang::ASTContext *Context, const std::vector<int> &Lines, const int CoverageToolId, const std::vector<DiffParser *> *DiffParserVector)
    : Context(Context), Lines(Lines), CoverageToolId(CoverageToolId), DiffParserVector(DiffParserVector){};

bool DiffASTVisitor::VisitStmt(clang::Stmt *s)
{
    if (Index < Lines.size() && Lines[Index] == Context->getSourceManager().getSpellingLineNumber(s->getBeginLoc()))
    {
        bool parsed = false;
        for (auto diffParser : *DiffParserVector)
        {
            if (diffParser->getFileTypeId() == CoverageToolId && diffParser->parse(s, Context))
            {
                llvm::outs() << "[" << diffParser->getCoverageTool() << "] "
                             << diffParser->getDescription() << " (" << diffParser->getFileType() << "@" << Lines[Index] << ")"
                             << "\n";
                parsed = true;
                break;
            }
        }
        // if (!clang::isa<clang::DeclStmt>(s))
        // {
        //     // Write to file
        //     llvm::outs() << "Delete Decl Stmt"
        //                  << " (" << (CoverageToolId == 0 ? "gcov" : "llvm-cov") << "@" << Lines[Index] << ")"
        //                  << "\n";
        // }
        Index++;
    }
    return true;
}