#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/prepared_statement.h>
#include <mysql_connection.h>
#include <mysql_driver.h>

#include <fstream>

#include "CoverageParser.h"
#include "EMIConst.h"
#include "EMIFrontendAction.h"
#include "EMIFrontendActionFactory.h"
#include "EMIUtil.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

static llvm::cl::OptionCategory EMIOptionCategory("EMI Options");
// EMI prune method option - Specify the method of EMI pruning
static llvm::cl::opt<int> MethodOption("m", llvm::cl::desc("EMI prune method"), llvm::cl::cat(EMIOptionCategory));
// Output directory option - Specify the output directory of the generated EMI file
static llvm::cl::opt<std::string> OutputOption("o", llvm::cl::desc("Explicitly specify a output directory"), llvm::cl::cat(EMIOptionCategory));

static llvm::cl::opt<int> TaskIdOption("id", llvm::cl::desc("Task Id option"), llvm::cl::cat(EMIOptionCategory));
static llvm::cl::opt<std::string> HostOption("h", llvm::cl::desc("MySQL host option"), llvm::cl::cat(EMIOptionCategory));
static llvm::cl::opt<int> PortOption("port", llvm::cl::desc("MySQL port option"), llvm::cl::cat(EMIOptionCategory));
static llvm::cl::opt<std::string> UserOption("u", llvm::cl::desc("MySQL user option"), llvm::cl::cat(EMIOptionCategory));
static llvm::cl::opt<std::string> PasswordOption("pwd", llvm::cl::desc("MySQL password option"), llvm::cl::cat(EMIOptionCategory));

int main(int argc, const char *argv[]) {
  clang::tooling::CommonOptionsParser op(argc, argv, EMIOptionCategory);
  const auto &sources = op.getSourcePathList();
  if (sources.size() != 1) {
    llvm::outs() << *argv << " can only handle one file argument!\n";
    return 1;
  }
  clang::tooling::ClangTool Tool(op.getCompilations(), sources);

  CoverageParser gcovParser = CoverageParser(parser::gcov::executed, parser::gcov::unexecuted, parser::gcov::isCountBeforeLineNum);
  CoverageParser llvmcovParser = CoverageParser(parser::llvmcov::executed, parser::llvmcov::unexecuted, parser::llvmcov::isCountBeforeLineNum);

  // ClangTool::run accepts a FrontendActionFactory, which is then used to
  // create new objects implementing the FrontendAction interface.
  Tool.run(newEMIFrontendActionFactory<GCovFrontendAction>(MethodOption, OutputOption, gcovParser).get());
  Tool.run(newEMIFrontendActionFactory<LLVMCovFrontendAction>(MethodOption, OutputOption, llvmcovParser).get());

  std::vector<int> diffLines;
  util::diffExecutedMaps(gcovParser.getCountMap(), llvmcovParser.getCountMap(), diffLines);
  if (diffLines.size() > 0) {
    if (TaskIdOption.getValue()) {
      sql::ConnectOptionsMap ConnProperties;
      if (!HostOption.empty()) {
        ConnProperties["hostName"] = HostOption;
      }
      if (!UserOption.empty()) {
        ConnProperties["userName"] = UserOption;
      }
      if (!PasswordOption.empty()) {
        ConnProperties["password"] = PasswordOption;
      }
      if (PortOption != 0) {
        ConnProperties["port"] = PortOption.getValue();
      }
      sql::Driver *driver = get_driver_instance();
      sql::Connection *conn = driver->connect(ConnProperties);
      conn->setSchema("covemidiff");
      std::stringstream sstream;
      sstream << "INSERT INTO diff_lines (task_id, method, line_num) VALUES ";
      for (int i = 0; i < diffLines.size() - 1; i++) {
        sstream << "(?, ?, ?), ";
      }
      sstream << "(?, ?, ?)";
      sql::PreparedStatement *prep_stmt = conn->prepareStatement(sstream.str());

      for (int index = 0; index < diffLines.size(); index++) {
        int base = 3 * index;
        prep_stmt->setInt(base + 1, TaskIdOption);
        prep_stmt->setInt(base + 2, MethodOption);
        prep_stmt->setInt(base + 3, diffLines[index]);
      }
      prep_stmt->executeUpdate();
      delete prep_stmt;
      delete conn;
    } else {
      std::string append = OutputOption;
      auto path = util::pathAppend(std::filesystem::path(sources[0]).parent_path(), append) / "diffLines";
      std::ofstream ofs(path);
      for (auto line : diffLines) {
        ofs << line << "\n";
      }
      ofs.close();
      llvm::outs() << "Diff lines file location: " + path.string();
    }
  }
}