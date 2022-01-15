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
    int getCount();

private:
    int CoverageToolId; // Diff bug is at that coverage tool's ID
    int FileTypeId;     // In which coverage tool's EMI file type can the Diff bug be found
    std::string Description;

protected:
    int Count = 0;
};

class UnmarkedLabelDiffParser : public DiffParser
{
public:
    explicit UnmarkedLabelDiffParser();
    virtual bool parse(const clang::Stmt *s, clang::ASTContext *Context) override;

private:
    const clang::Stmt *UnmarkedLabelStmt;
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

private:
    const clang::Stmt *IfOptimizeStmt;
};

class JumpBlockDiffParser : public DiffParser
{
public:
    explicit JumpBlockDiffParser();
    virtual bool parse(const clang::Stmt *s, clang::ASTContext *Context) override;

private:
    const clang::Stmt *PreJumpBlockStmt;
};

const std::vector<DiffParser *> *createDiffParserVector();