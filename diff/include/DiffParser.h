#pragma once

#include "clang/AST/Stmt.h"
#include "clang/AST/ASTContext.h"

// Parse the AST tree of inconsistent lines in the file and determine whether it is caused by a specific Diff Bug
class DiffParser
{
public:
    DiffParser(const int &CoverageToolId, const int &FileTypeId, const std::string &Description);
    virtual bool parse(const clang::Stmt *s, clang::ASTContext *Context) = 0;
    const int getFileTypeId() const;
    const int getCoverageToolId() const;
    const std::string &getDescription() const;
    const int getCount() const;

private:
    const int &CoverageToolId; // Diff bug is at that coverage tool's ID
    const int &FileTypeId;     // In which coverage tool's EMI file type can the Diff bug be found
    const std::string &Description;

protected:
    int Count = 0;
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

class JumpBlockDiffParser : public DiffParser
{
public:
    explicit JumpBlockDiffParser();
    virtual bool parse(const clang::Stmt *s, clang::ASTContext *Context) override;

private:
    const clang::Stmt *PreJumpBlockStmt;
};

const std::vector<DiffParser *> *createDiffParserVector();