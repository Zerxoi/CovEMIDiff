#pragma once

#include "EMIConst.h"
#include "clang/Tooling/Tooling.h"

template <typename T>
class EMIFrontendActionFactory : public clang::tooling::FrontendActionFactory {
 public:
  EMIFrontendActionFactory(const std::string &CoverageToolVersion, const int MethodOption, const std::string &OutputOption)
      : CoverageToolVersion(CoverageToolVersion), MethodOption(MethodOption), OutputOption(OutputOption) {}

  std::unique_ptr<clang::FrontendAction> create() override {
    return std::make_unique<T>(CoverageToolVersion, MethodOption, OutputOption);
  }

 private:
  const std::string &CoverageToolVersion;
  const int MethodOption;
  const std::string &OutputOption;
};

// Customize FrontendActionFactory based on Method Option and Output Option
template <typename T>
std::unique_ptr<clang::tooling::FrontendActionFactory> newEMIFrontendActionFactory(const std::string &CoverageToolVersion, const int MethodOption, const std::string &OutputOption) {
  if (MethodOption != method::pre && MethodOption != method::post) {
    throw std::runtime_error("Please make the correct method options, 0 means pre, 1 means post");
  }
  return std::unique_ptr<clang::tooling::FrontendActionFactory>(new EMIFrontendActionFactory<T>(CoverageToolVersion, MethodOption, OutputOption));
};