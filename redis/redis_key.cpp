#include "redis_key.h"
#include "redis_helper.h"

int redis_key::del(const std::vector<std::string>& keys)
{
    std::string key_list = redis_helper::join(keys);
    build_command("DEL %s", key_list.c_str());

    if (!keys.empty()) {
        hash_slots(keys[0]);
    }

    return get_integer32();
}

