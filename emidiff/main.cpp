#include "cppconn/connection.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CommonOptionsParser.h"

#include "DiffFrontendActionFactory.h"

static llvm::cl::OptionCategory DiffOptionCategory("Diff Options");
static llvm::cl::opt<int> TaskIdOption("id", llvm::cl::desc("Task Id option"), llvm::cl::cat(DiffOptionCategory));
static llvm::cl::opt<int> MethodOption("m", llvm::cl::desc("EMI prune method option"), llvm::cl::cat(DiffOptionCategory));
static llvm::cl::opt<std::string> HostOption("h", llvm::cl::desc("MySQL host option"), llvm::cl::cat(DiffOptionCategory));
static llvm::cl::opt<int> PortOption("port", llvm::cl::desc("MySQL port option"), llvm::cl::cat(DiffOptionCategory));
static llvm::cl::opt<std::string> UserOption("u", llvm::cl::desc("MySQL user option"), llvm::cl::cat(DiffOptionCategory));
static llvm::cl::opt<std::string> PasswordOption("pwd", llvm::cl::desc("MySQL password option"), llvm::cl::cat(DiffOptionCategory));

int main(int argc, const char *argv[])
{
    clang::tooling::CommonOptionsParser op(argc, argv, DiffOptionCategory);
    const auto &sources = op.getSourcePathList();
    if (sources.size() != 2)
    {
        llvm::outs() << *argv << " compare the difference between two EMI programs.\n"
                     << "Please enter 2 file path parameters!\n";
        return 1;
    }

    sql::ConnectOptionsMap ConnProperties;
    if (!HostOption.empty())
    {
        ConnProperties["hostName"] = HostOption;
    }
    if (!UserOption.empty())
    {
        ConnProperties["userName"] = UserOption;
    }
    if (!PasswordOption.empty())
    {
        ConnProperties["password"] = PasswordOption;
    }
    if (PortOption != 0)
    {
        ConnProperties["port"] = PortOption.getValue();
    }

    clang::tooling::ClangTool Tool(op.getCompilations(), sources);
    Tool.run(newDiffFrontendActionFactory(sources[0], sources[1], TaskIdOption, MethodOption, ConnProperties).get());
}