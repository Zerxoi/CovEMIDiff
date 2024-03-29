#pragma once

#include <cppconn/connection.h>

#include <filesystem>

#include "DiffParser.h"
#include "clang/Tooling/Tooling.h"

class DiffFrontendActionFactory : public clang::tooling::FrontendActionFactory {
 public:
  DiffFrontendActionFactory(const std::string &gcovFile, const std::string &llvmcovFile, const int DiffIdOption, sql::ConnectOptionsMap &ConnProperties);

  std::unique_ptr<clang::FrontendAction> create() override;

 private:
  std::vector<int> gcovLines;
  std::vector<int> llvmcovLines;
  const int DiffIdOption;
  const std::vector<DiffParser *> *DiffParserVector;
  sql::ConnectOptionsMap &ConnProperties;
  bool isGcovLines = true;
};

std::unique_ptr<clang::tooling::FrontendActionFactory> newDiffFrontendActionFactory(const std::string &gcovFile, const std::string &llvmcovFile, const int DiffIdOption, sql::ConnectOptionsMap &conn_properties);