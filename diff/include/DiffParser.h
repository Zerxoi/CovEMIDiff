#pragma once

#include "clang/AST/Stmt.h"
#include "clang/AST/ASTContext.h"

class DiffParser
{
public:
    DiffParser(int CoverageToolId, int FileTypeId, std::string Description);
    virtual bool parse(const clang::Stmt *s, clang::ASTContext *Context) = 0;
    int getFileTypeId();
    std::string getFileType();
    int getCoverageToolId();
    std::string getCoverageTool();
    const std::string &getDescription();

private:
    int CoverageToolId;
    int FileTypeId;
    std::string Description;
};

class UnmarkedLabelDiffParser : public DiffParser
{
public:
    explicit UnmarkedLabelDiffParser();
    virtual bool parse(const clang::Stmt *s, clang::ASTContext *Context) override;
};

class ConstArrayInitializationDiffParser : public DiffParser
{
public:
    explicit ConstArrayInitializationDiffParser();
    virtual bool parse(const clang::Stmt *s, clang::ASTContext *Context) override;
};

class IfOptimizeDiffParser : public DiffParser
{
public:
    explicit IfOptimizeDiffParser();
    virtual bool parse(const clang::Stmt *s, clang::ASTContext *Context) override;
    // Wraps the clang::Expr's isEvaluatable method
    bool isEvaluatable(const clang::Expr *expr, const clang::ASTContext &Context);
};

class JumpBlockDiffParser : public DiffParser
{
public:
    explicit JumpBlockDiffParser();
    virtual bool parse(const clang::Stmt *s, clang::ASTContext *Context) override;
    bool containsBlockJumpStmt(const clang::Stmt *s);
    bool isBlockJumpStmt(const clang::Stmt *s);

private:
    const clang::Stmt *PreBlockJumpStmt;
};

const std::vector<DiffParser *> *createDiffParserVector();