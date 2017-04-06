#include "redis_zset.h"
#include "redis_helper.h"

long long redis_zset::zadd(const std::string& key,
                           const std::map<double, std::string>& score_mems,
                           int nx_or_xx /*= 0*/, bool is_change /*= false*/)
{
    std::string score_mems_list = redis_helper::join(score_mems);

    /* ZADD key [NX|XX] [CH] score member [score member ...] */
    build_command("ZADD %s %s %s %s", key.c_str(),
                  nx_or_xx > 0 ? "XX" : (nx_or_xx < 0 ? "NX" : ""),
                  is_change ? "CH" : "", score_mems_list.c_str());
    hash_slots(key);

    return get_integer64();
}

long long redis_zset::zadd_nx(const std::string& key,
                              const std::map<double,std::string>& score_mems,
                              bool is_change /*= false*/)
{
    return zadd(key, score_mems, -1, is_change);
}

long long redis_zset::zadd_xx(const std::string& key,
                              const std::map<double,std::string>& score_mems,
                              bool is_change /*= false*/)
{
    return zadd(key, score_mems, 1, is_change);
}

long long redis_zset::zadd_ch(const std::string& key,
                              const std::map<double,std::string>& score_mems,
                              int nx_or_xx /*= 0*/)
{
    return zadd(key, score_mems, nx_or_xx);
}

bool redis_zset::zadd_incr(const std::string& key, double score,
                           const std::string& member, std::string& result)
{
    build_command("ZADD %s INCR %lf %s", key.c_str(), score, member.c_str());
    hash_slots(key);

    return get_string(result);
}

long long redis_zset::zcard(const std::string& key)
{
    build_command("ZCARD %s", key.c_str());
    hash_slots(key);

    return get_integer64();
}

long long redis_zset::zcount(const std::string& key, double min, double max)
{
    build_command("ZCOUNT %s %lf %lf", key.c_str(), min, max);
    hash_slots(key);

    return get_integer64();
}

bool redis_zset::zincrby(const std::string& key, double increment,
                         const std::string& member, std::string& result)
{
    build_command("ZINCRBY %s %lf %s", key.c_str(), increment, member.c_str());
    hash_slots(key);

    return get_string(result);
}

long long redis_zset::zinterstore(const std::string& dest, int numkeys,
                                  const std::vector<std::string>& keys,
                                  const std::vector<double>& weights,
                                  const char* aggregate /*= NULL*/)
{
    return zstore("ZINTERSTORE", dest, numkeys, keys, weights, aggregate);
}

long long redis_zset::zunionstore(const std::string& dest, int numkeys,
                                  const std::vector<std::string>& keys,
                                  const std::vector<double>& weights,
                                  const char* aggregate /*= "SUM"*/)
{
    return zstore("ZUNIONSTORE", dest, numkeys, keys, weights, aggregate);
}

long long redis_zset::zstore(const char* cmd, const std::string& dest,
                             int numkeys, const std::vector<std::string>& keys,
                             const std::vector<double>& weights,
                             const char* aggregate)
{
    std::string keys_list = redis_helper::join(keys);
    std::string weights_list("WEIGHTS");
    weights_list += redis_helper::join(weights);

    build_command("%s %s %d %s %s AGGREGATE %s",
                  cmd, dest.c_str(), numkeys, keys_list.c_str(),
                  weights.size() > 0 ? weights_list.c_str() : "", aggregate);
    hash_slots(dest);

    return get_integer64();
}

