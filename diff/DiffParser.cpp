#include "clang/AST/Expr.h"

#include "Util.h"
#include "DiffParser.h"

DiffParser::DiffParser(int CoverageToolId, int FileTypeId, std::string Description) : CoverageToolId(CoverageToolId), FileTypeId(FileTypeId), Description(Description) {}

int DiffParser::getFileTypeId()
{
    return FileTypeId;
}

std::string DiffParser::getFileType()
{
    return util::idToString(FileTypeId);
}

int DiffParser::getCoverageToolId()
{
    return CoverageToolId;
}

std::string DiffParser::getCoverageTool()
{
    return util::idToString(CoverageToolId);
}

const std::string &DiffParser::getDescription()
{
    return Description;
}

int DiffParser::getCount()
{
    return Count;
}

UnmarkedLabelDiffParser::UnmarkedLabelDiffParser() : UnmarkedLabelStmt(nullptr), DiffParser(0, 1, "Unmarked Label"){};

bool UnmarkedLabelDiffParser::parse(const clang::Stmt *s, clang::ASTContext *Context)
{
    if (clang::isa<clang::LabelStmt>(s))
    {
        if (!util::bfs(UnmarkedLabelStmt, [s](auto child)
                       { return child == s; }))
        {
            UnmarkedLabelStmt = s;
            Count++;
        }
        return true;
    }
    if (util::bfs(UnmarkedLabelStmt, [s](auto child)
                  { return child == s; }))
    {
        return true;
    }
    return false;
}

ConstArrayInitializationDiffParser::ConstArrayInitializationDiffParser() : DiffParser(0, 0, "Const Array Initialization"){};

bool ConstArrayInitializationDiffParser::parse(const clang::Stmt *s, clang::ASTContext *Context)
{
    if (auto declStmt = clang::dyn_cast<clang::DeclStmt>(s))
    {
        for (auto decl : declStmt->decls())
        {
            if (auto *varDecl = clang::dyn_cast<clang::VarDecl>(decl))
            {
                if (varDecl->hasInit() && clang::isa<clang::ConstantArrayType>(varDecl->getType()) && varDecl->getType().isConstQualified())
                {
                    Count++;
                    return true;
                }
            }
        }
    }
    return false;
}

IfOptimizeDiffParser::IfOptimizeDiffParser() : IfOptimizeStmt(nullptr), DiffParser(0, 0, "If Optimize") {}

bool IfOptimizeDiffParser::isEvaluatable(const clang::Expr *expr, const clang::ASTContext &Context)
{
    if (auto binOpr = clang::dyn_cast<clang::BinaryOperator>(expr))
    {
        if (binOpr->isCommaOp())
        {
            return isEvaluatable(binOpr->getRHS(), Context);
        }
    }
    if (auto parenOpr = clang::dyn_cast<clang::ParenExpr>(expr))
    {
        return isEvaluatable(parenOpr->getSubExpr(), Context);
    }
    return expr->isEvaluatable(Context);
}

bool IfOptimizeDiffParser::parse(const clang::Stmt *s, clang::ASTContext *Context)
{
    if (auto ifStmt = clang::dyn_cast<clang::IfStmt>(s))
    {
        if (isEvaluatable(ifStmt->getCond(), *Context))
        {
            if (!util::bfs(IfOptimizeStmt, [s](auto child)
                           { return child == s; }))
            {
                IfOptimizeStmt = s;
                Count++;
            }
            return true;
        }
    }
    if (util::bfs(IfOptimizeStmt, [s](auto child)
                  { return child == s; }))
    {
        return true;
    }
    return false;
}

JumpBlockDiffParser::JumpBlockDiffParser() : PreJumpBlockStmt(nullptr), DiffParser(1, 1, "Jump Block") {}

bool JumpBlockDiffParser::parse(const clang::Stmt *s, clang::ASTContext *Context)
{
    auto preStmt = util::getSiblingStmt(s, -1, Context);
    if (PreJumpBlockStmt != nullptr && util::bfs(preStmt, [this](const clang::Stmt *child)
                                                 { return child == PreJumpBlockStmt; }))
    {
        PreJumpBlockStmt = s;
        return true;
    }
    if (util::bfs(preStmt, [](const clang::Stmt *child)
                  { return child != nullptr && (clang::isa<clang::ReturnStmt>(child) || clang::isa<clang::BreakStmt>(child) || clang::isa<clang::ContinueStmt>(child)); }))
    {
        Count++;
        PreJumpBlockStmt = s;
        return true;
    }
    return false;
}

const std::vector<DiffParser *> *createDiffParserVector()
{
    std::vector<DiffParser *> *vector = new std::vector<DiffParser *>;
    vector->push_back(new IfOptimizeDiffParser);
    vector->push_back(new UnmarkedLabelDiffParser);
    vector->push_back(new ConstArrayInitializationDiffParser);
    vector->push_back(new JumpBlockDiffParser);

    return vector;
}