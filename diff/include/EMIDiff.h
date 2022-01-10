#pragma once

#include <vector>
#include <string>

void EMIDiff(const std::string gcovFile, const std::string llvmcovFile, std::vector<int> &gcovVector, std::vector<int> &llvmcovVector);