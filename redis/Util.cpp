#include "Util.h"

void Util::split(const std::string& str, const std::string& separator,
    std::vector<std::string>& list)
{
    size_t cur = 0;
    size_t last = 0;
    while (pos != std::string::npos) {
        cur = str.find(separator);
        list.push_back(str.substr(last, cur));
    }
}
