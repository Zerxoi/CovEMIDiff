#include <fstream>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>

#include "DiffUtil.h"
#include "DiffConst.h"
#include "DiffASTConsumer.h"
#include "DiffFrontendAction.h"

DiffFrontendAction::DiffFrontendAction(const std::vector<int> &gcovLines, const std::vector<int> &llvmcovLines, const int TaskIdOption, const int MethodOption, const std::vector<DiffParser *> *DiffParserVector, sql::ConnectOptionsMap &ConnProperties)
    : gcovLines(gcovLines), llvmcovLines(llvmcovLines), TaskIdOption(TaskIdOption), MethodOption(MethodOption), DiffParserVector(DiffParserVector), ConnProperties(ConnProperties){};

std::unique_ptr<clang::ASTConsumer> DiffFrontendAction::CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile)
{
    const std::vector<int> *lines;
    if (InFile.contains(extension::gcov))
    {
        CoverageTool = gcov;
        lines = &gcovLines;
    }
    else if (InFile.contains(extension::llvmcov))
    {
        CoverageTool = llvmcov;
        lines = &llvmcovLines;
    }
    else
    {
        throw std::runtime_error("Unable to tell if the file was generated by gcov or llvm-cov");
    }
    return std::make_unique<DiffASTConsumer>(&Compiler.getASTContext(), *lines, CoverageTool, InFile, DiffParserVector, DiffReasonVector);
}

void DiffFrontendAction::EndSourceFileAction()
{
    std::string coverageTool;
    const std::vector<int> *lines;
    if (CoverageTool == gcov)
    {
        lines = &gcovLines;
    }
    else if (CoverageTool == llvmcov)
    {
        lines = &llvmcovLines;
    }
    else
    {
        throw std::runtime_error("Cannot handle CoverageTool with value " + CoverageTool);
    }

    if (DiffReasonVector.size() > 0)
    {

        sql::Driver *driver = get_driver_instance();
        sql::Connection *conn = driver->connect(ConnProperties);
        conn->setSchema("covemidiff");
        std::stringstream sstream;
        sstream << "INSERT INTO diff (task_id, method, file_type_id, line_num, reason, coverage_tool_id, count) VALUES ";
        for (int i = 0; i < lines->size() - 1; i++)
        {
            sstream << "(?, ?, ?, ?, ?, ?, ?), ";
        }
        sstream << "(?, ?, ?, ?, ?, ?, ?)";
        sql::PreparedStatement *prep_stmt = conn->prepareStatement(sstream.str());

        int index = 0;
        for (; index < DiffReasonVector.size(); index++)
        {
            int base = 7 * index;
            const DiffReason *reason = DiffReasonVector.at(index);
            prep_stmt->setInt(base + 1, TaskIdOption);
            prep_stmt->setInt(base + 2, MethodOption);
            prep_stmt->setInt(base + 3, CoverageTool);
            prep_stmt->setInt(base + 4, reason->getLineNum());
            prep_stmt->setString(base + 5, reason->getDescription());
            if (reason->getParser() != nullptr)
            {
                auto parser = reason->getParser();
                prep_stmt->setInt(base + 6, parser->getCoverageTool());
                prep_stmt->setInt(base + 7, parser->getCount());
            }
            else
            {
                prep_stmt->setInt(base + 6, 0);
                prep_stmt->setInt(base + 7, 0);
            }
        }
        while (index < lines->size())
        {
            int base = 7 * index;
            prep_stmt->setInt(base + 1, TaskIdOption);
            prep_stmt->setInt(base + 2, MethodOption);
            prep_stmt->setInt(base + 3, CoverageTool);
            prep_stmt->setInt(base + 4, lines->at(index++));
            prep_stmt->setString(base + 5, reason::description::terminated);
            prep_stmt->setInt(base + 6, 0);
            prep_stmt->setInt(base + 7, 0);
        }
        prep_stmt->executeUpdate();
        delete prep_stmt;
        delete conn;
    }
}