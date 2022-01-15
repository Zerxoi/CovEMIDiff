#pragma once

#include <vector>
#include <string>

// Compare the file lines of 'gcovFile' and 'llvmcovFile', put the inconsistent lines of 'gcovFile'
// into 'gcovVector', and put the inconsistent lines of 'llvmcovFile' into 'llvmcovVector'
void EMIDiff(const std::string gcovFile, const std::string llvmcovFile, std::vector<int> &gcovVector, std::vector<int> &llvmcovVector);