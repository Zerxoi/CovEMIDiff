#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "EMIFrontendAction.h"

static llvm::cl::OptionCategory EMIOptionCategory("EMI Options");
static llvm::cl::opt<int> EMIMethod("m", llvm::cl::desc("EMI prune method"), llvm::cl::cat(EMIOptionCategory));

template <typename T>
std::unique_ptr<clang::tooling::FrontendActionFactory> EMINewFrontendActionFactory(int MethodOption)
{
    class SimpleFrontendActionFactory : public clang::tooling::FrontendActionFactory
    {
    public:
        SimpleFrontendActionFactory(int MethodOption) : MethodOption(MethodOption) {}

        std::unique_ptr<clang::FrontendAction> create() override
        {
            return std::make_unique<T>(MethodOption);
        }

    private:
        int MethodOption;
    };

    return std::unique_ptr<clang::tooling::FrontendActionFactory>(new SimpleFrontendActionFactory(MethodOption));
}

int main(int argc, const char *argv[])
{
    clang::tooling::CommonOptionsParser op(argc, argv, EMIOptionCategory);
    clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());
    int method = EMIMethod.getValue();

    // ClangTool::run accepts a FrontendActionFactory, which is then used to
    // create new objects implementing the FrontendAction interface.
    Tool.run(EMINewFrontendActionFactory<GCovFrontendAction>(method).get());
    Tool.run(EMINewFrontendActionFactory<LLVMCovFrontendAction>(method).get());
}