#include "redis_helper.h"
#include "redis_log.h"

std::string redis_helper::join(const std::vector<std::string>& list,
                               const std::string & separator /*= " "*/)
{
    std::string str("");
    for (size_t i = 0; i < list.size(); i++) {
        str += list[i] + ((i<list.size()-1) ? separator : "");
    }

    return str;
}

std::string redis_helper::join(const std::map<std::string,std::string>& map,
                               const std::string & separator_key_value /*= " "*/,
                               const std::string & separator /*= " "*/)
{
    std::string str("");
    std::map<std::string, std::string>::const_iterator it = map.begin();
    std::map<std::string, std::string>::const_iterator last = map.end();
    last--;
    for (; it != map.end(); ++it) {
        str += it->first + separator_key_value + it->second
               + ((it != last) ? separator : "");
    }

    return str;
}

