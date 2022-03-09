#include "DiffFrontendActionFactory.h"

#include <filesystem>

#include "DiffFrontendAction.h"
#include "EMIDiff.h"

DiffFrontendActionFactory::DiffFrontendActionFactory(const std::string &gcovFile, const std::string &llvmcovFile, const int DiffIdOption, sql::ConnectOptionsMap &ConnProperties)
    : DiffIdOption(DiffIdOption), ConnProperties(ConnProperties) {
  EMIDiff(gcovFile, llvmcovFile, gcovLines, llvmcovLines);
  if (DiffIdOption <= 0) {
    throw std::runtime_error("Please give a valid Task Id value");
  }
  DiffParserVector = createDiffParserVector();
}

std::unique_ptr<clang::FrontendAction> DiffFrontendActionFactory::create() {
  std::vector<int> *lines;
  if (isGcovLines) {
    lines = &gcovLines;
  } else {
    lines = &llvmcovLines;
  }
  isGcovLines = !isGcovLines;
  return std::make_unique<DiffFrontendAction>(*lines, DiffIdOption, DiffParserVector, ConnProperties);
}

std::unique_ptr<clang::tooling::FrontendActionFactory> newDiffFrontendActionFactory(const std::string &gcovFile, const std::string &llvmcovFile, const int DiffIdOption, sql::ConnectOptionsMap &ConnProperties) {
  return std::unique_ptr<clang::tooling::FrontendActionFactory>(new DiffFrontendActionFactory(gcovFile, llvmcovFile, DiffIdOption, ConnProperties));
}