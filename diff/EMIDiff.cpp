#include <filesystem>
#include <fstream>
#include <cctype>

#include "dtl/dtl.hpp"
#include "EMIDiff.h"

bool isEmptyLine(const std::string &line)
{
    if (line.empty())
    {
        return true;
    }
    int i = 0;
    for (; i < line.size() && std::isspace(line[i]); i++)
        ;
    if (i == line.size() || line[i] == '{' || line[i] == '}')
    {
        return true;
    }
    return false;
}

void EMIDiff(const std::string gcovFile, const std::string llvmcovFile, std::vector<int> &gcovVector, std::vector<int> &llvmcovVector)
{
    if (!std::filesystem::exists(gcovFile) || !std::filesystem::exists(llvmcovFile))
    {
        throw std::runtime_error("EMI files do not exist");
    }

    std::ifstream gcovIfs(gcovFile);
    std::ifstream llvmcovIfs(llvmcovFile);
    std::string buf;
    std::vector<std::string> gcovLines, llvmcovLines;

    while (getline(gcovIfs, buf))
    {
        gcovLines.push_back(buf);
    }
    while (getline(llvmcovIfs, buf))
    {
        llvmcovLines.push_back(buf);
    }

    dtl::Diff<std::string> diff(gcovLines, llvmcovLines);
    diff.onHuge();
    diff.compose();

    for (auto &se : diff.getSes().getSequence())
    {
        if (se.second.type == 1 && !isEmptyLine(llvmcovLines[se.second.afterIdx - 1]))
        {
            llvmcovVector.push_back(se.second.afterIdx);
        }
        else if (se.second.type == -1 && !isEmptyLine(gcovLines[se.second.beforeIdx - 1]))
        {
            gcovVector.push_back(se.second.beforeIdx);
        }
    }
}