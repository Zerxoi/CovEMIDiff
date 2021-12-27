#include <map>
#include <regex>

class CovParser
{
private:
    const std::string separator;
    const int countIndex;
    const int numIndex;
    std::regex re;

public:
    CovParser(std::string separator, int countIndex, int numIndex);
    std::map<int, int> *parse(std::string file);
};
