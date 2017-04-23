#include <stdlib.h>

#include "redis_zset.h"
#include "redis_helper.h"
#include "redis_log.h"

long long redis_zset::zadd(const std::string& key,
    const std::vector< std::pair<std::string, double> >& member_score,
    int nx_or_xx /*= 0*/, bool is_change /*= false*/)
{
    std::vector<std::string> argv;
    argv.push_back("ZADD");
    argv.push_back(key.c_str());

    if (nx_or_xx > 0) {
        argv.push_back("XX");
    }
    else if (nx_or_xx < 0) {
        argv.push_back("NX");
    }

    if (is_change) {
        argv.push_back("CH");
    }

    for (size_t i = 0; i < member_score.size(); i++) {
        argv.push_back(TO_STRING(member_score[i].second));
        argv.push_back(member_score[i].first);
    }

    build_request(argv);
    hash_slots(key);

    return get_integer64();
}

long long redis_zset::zadd_nx(const std::string& key,
    const std::vector< std::pair<std::string, double> >& member_score,
    bool is_change /*= false*/)
{
    return zadd(key, member_score, -1, is_change);
}

long long redis_zset::zadd_xx(const std::string& key,
    const std::vector< std::pair<std::string, double> >& member_score,
    bool is_change /*= false*/)
{
    return zadd(key, member_score, 1, is_change);
}

long long redis_zset::zadd_ch(const std::string& key,
    const std::vector< std::pair<std::string, double> >& member_score,
    int nx_or_xx /*= 0*/)
{
    return zadd(key, member_score, nx_or_xx, true);
}

bool redis_zset::zadd_incr(const std::string& key, double score,
                           const std::string& member, std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("ZADD");
    argv.push_back(key.c_str());
    argv.push_back("INCR");
    argv.push_back(TO_STRING(score));
    argv.push_back(member.c_str());

    build_request(argv);
    hash_slots(key);

    return get_string(result);
}

long long redis_zset::zcard(const std::string& key)
{
    std::vector<std::string> argv;
    argv.push_back("ZCARD");
    argv.push_back(key.c_str());

    build_request(argv);
    hash_slots(key);

    return get_integer64();
}

long long redis_zset::zcount(const std::string& key, double min, double max)
{
    return zcount(key, TO_STRING(min), TO_STRING(max));
}

long long redis_zset::zcount(const std::string& key, const std::string& min,
                             const std::string& max)
{
    std::vector<std::string> argv;
    argv.push_back("ZCOUNT");
    argv.push_back(key.c_str());
    argv.push_back(min.c_str());
    argv.push_back(max.c_str());

    build_request(argv);
    hash_slots(key);

    return get_integer64();
}

bool redis_zset::zincrby(const std::string& key, double increment,
                         const std::string& member,
                         std::string* result /*= NULL*/)
{
    std::vector<std::string> argv;
    argv.push_back("ZINCRBY");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(increment));
    argv.push_back(member.c_str());

    build_request(argv);
    hash_slots(key);

    return get_string(result);
}

long long redis_zset::zstore(const char* cmd, const std::string& dest,
                             const std::vector<std::string>& keys,
                             const std::vector<double>* weights,
                             const char* aggregate)
{
    std::vector<std::string> argv;
    argv.push_back(cmd);
    argv.push_back(dest.c_str());
    argv.push_back(TO_STRING(keys.size()));

    for (size_t i = 0; i < keys.size(); i++) {
        argv.push_back(keys[i]);
    }

    if (weights != NULL && weights->size() > 0) {
        argv.push_back("WEIGHTS");
        for (size_t i = 0; i < weights->size(); i++) {
            argv.push_back(TO_STRING((*weights)[i]));
        }
    }

    if (aggregate != NULL) {
        argv.push_back("AGGREGATE");
        argv.push_back(aggregate);
    }

    build_request(argv);
    hash_slots(dest);

    return get_integer64();
}

long long redis_zset::zinterstore(const std::string& dest,
                                  const std::vector<std::string>& keys,
                                  const std::vector<double>* weights /*= NULL*/,
                                  const char* aggregate /*= "SUM"*/)
{
    return zstore("ZINTERSTORE", dest, keys, weights, aggregate);
}

long long redis_zset::zunionstore(const std::string& dest,
                                  const std::vector<std::string>& keys,
                                  const std::vector<double>* weights /*= NULL*/,
                                  const char* aggregate /*= "SUM"*/)
{
    return zstore("ZUNIONSTORE", dest, keys, weights, aggregate);
}

long long redis_zset::zlexcount(const std::string& key,
                                const std::string& min,
                                const std::string& max)
{
    std::vector<std::string> argv;
    argv.push_back("ZLEXCOUNT");
    argv.push_back(key.c_str());
    argv.push_back(min.c_str());
    argv.push_back(max.c_str());

    build_request(argv);
    hash_slots(key);

    return get_integer64();
}

int redis_zset::zrange(const std::string& key, int start, int stop,
                       std::vector<std::string>& result)
{
    std::vector<std::string> argv;
    argv.push_back("ZRANGE");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(start));
    argv.push_back(TO_STRING(stop));

    build_request(argv);
    hash_slots(key);

    return get_array_or_nil(result);
}

int redis_zset::zrange_with_scores(const std::string& key,
    int start, int stop,
    std::vector< std::pair<std::string, double> >& result)
{
    std::vector<std::string> argv;
    argv.push_back("ZRANGE");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(start));
    argv.push_back(TO_STRING(stop));
    argv.push_back("WITHSCORES");

    build_request(argv);
    hash_slots(key);

    std::vector<std::string> elem_score;
    int count = get_array_or_nil(elem_score);

    return (count > 0) ? parse_element_score(elem_score, result) : count;
}

int redis_zset::parse_element_score(const std::vector<std::string>& in,
    std::vector< std::pair<std::string, double> >& out)
{
    if (in.size() % 2 != 0) {
        return -1;
    }

    for (size_t i = 0; i < in.size(); i += 2) {
        double score = atof(in[i+1].c_str());
        out.push_back(std::make_pair(in[i], score));
    }

    return in.size() / 2;
}

int redis_zset::zrangeby(const char* cmd, const std::string& key,
                         const std::string& min, const std::string& max,
                         std::vector<std::string>& result,
                         int offset, int count, bool with_scores /*= false*/)
{
    std::vector<std::string> argv;
    argv.push_back(cmd);
    argv.push_back(key.c_str());
    argv.push_back(min.c_str());
    argv.push_back(max.c_str());

    if (offset >=0 && count > 0) {
        argv.push_back("LIMIT");
        argv.push_back(TO_STRING(offset));
        argv.push_back(TO_STRING(count));
    }
    if (with_scores) {
        argv.push_back("WITHSCORES");
    }

    build_request(argv);
    hash_slots(key);

    return get_array_or_nil(result);
}

int redis_zset::zrangebylex(const std::string& key,
                             const std::string& min,
                             const std::string& max,
                             std::vector<std::string>& result,
                             int offset /*= -1*/,int count /*= 0*/)
{
    return zrangeby("ZRANGEBYLEX", key, min, max, result, offset, count);
}

int redis_zset::zrangebyscore(const std::string& key, double min, double max,
                              std::vector<std::string>& result,
                              int offset /*= -1*/,int count /*= 0*/)
{
    return zrangeby("ZRANGEBYSCORE", key, TO_STRING(min),
                    TO_STRING(max), result, offset, count);
}

int redis_zset::zrangebyscore(const std::string& key,
                              const std::string& min,
                              const std::string& max,
                              std::vector<std::string>& result,
                              int offset /*= -1*/,int count /*= 0*/)
{
    return zrangeby("ZRANGEBYSCORE", key, min, max, result, offset, count);
}

int redis_zset::zrangebyscore_with_scores(const std::string& key,
    double min, double max,
    std::vector< std::pair<std::string, double> >& result,
    int offset /*= -1*/, int count /*= 0*/)
{
    std::vector<std::string> elem_score;
    int iret = zrangeby("ZRANGEBYSCORE", key, TO_STRING(min),
                        TO_STRING(max),elem_score, offset, count, true);

    return (iret > 0) ? parse_element_score(elem_score, result) : iret;
}

int redis_zset::zrangebyscore_with_scores(const std::string& key,
    const std::string& min, const std::string& max,
    std::vector< std::pair<std::string, double> >& result,
    int offset /*= -1*/, int count /*= 0*/)
{
    std::vector<std::string> elem_score;
    int iret = zrangeby("ZRANGEBYSCORE", key, min, max,
                         elem_score, offset, count, true);

    return (iret > 0) ? parse_element_score(elem_score, result) : iret;
}

int redis_zset::zrank(const std::string& key, const std::string& member)
{
    std::vector<std::string> argv;
    argv.push_back("ZRANK");
    argv.push_back(key.c_str());
    argv.push_back(member.c_str());

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

long long redis_zset::zrem(const std::string& key,
                           const std::vector<std::string>& member)
{
    std::vector<std::string> argv;
    argv.push_back("ZREM");
    argv.push_back(key.c_str());

    for (size_t i = 0; i < member.size(); i++) {
        argv.push_back(member[i]);
    }

    build_request(argv);
    hash_slots(key);

    return get_integer64();
}

int redis_zset::zremrangebylex(const std::string& key, const std::string& min,
                               const std :: string & max)
{
    std::vector<std::string> argv;
    argv.push_back("ZREMRANGEBYLEX");
    argv.push_back(key.c_str());
    argv.push_back(min.c_str());
    argv.push_back(max.c_str());

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

int redis_zset::zremrangebyrank(const std::string& key, int start,int stop)
{
    std::vector<std::string> argv;
    argv.push_back("ZREMRANGEBYRANK");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(start));
    argv.push_back(TO_STRING(stop));

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

int redis_zset::zremrangebyscore(const std::string& key, const std::string& min,
                                 const std::string& max)
{
    std::vector<std::string> argv;
    argv.push_back("ZREMRANGEBYSCORE");
    argv.push_back(key.c_str());
    argv.push_back(min.c_str());
    argv.push_back(max.c_str());

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

int redis_zset::zremrangebyscore(const std::string& key, double min, double max)
{
    return zremrangebyscore(key, TO_STRING(min),TO_STRING(max));
}

int redis_zset::zrevrange(const std::string& key, int start, int stop,
                          std::vector<std::string>& result)
{
    std::vector<std::string> argv;
    argv.push_back("ZREVRANGE");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(start));
    argv.push_back(TO_STRING(stop));

    build_request(argv);
    hash_slots(key);

    return get_array_or_nil(result);
}

int redis_zset::zrevrange_with_scores(const std::string& key, int start,
    int stop, std::vector< std::pair<std::string, double> >& result)
{
    std::vector<std::string> argv;
    argv.push_back("ZREVRANGE");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(start));
    argv.push_back(TO_STRING(stop));
    argv.push_back("WITHSCORES");

    build_request(argv);
    hash_slots(key);

    std::vector<std::string> elem_score;
    int count = get_array_or_nil(elem_score);

    return (count > 0) ? parse_element_score(elem_score, result) : count;
}

int redis_zset::zrevrangebylex(const std::string& key, const std::string& min,
    const std::string& max, std::vector<std::string>& result,
    int offset /*= -1*/, int count /*= 0*/)
{
    return zrangeby("ZREVRANGEBYLEX", key, min, max, result, offset, count);
}

int redis_zset::zrevrangebyscore(const std::string& key, double min, double max,
    std::vector<std::string>& result, int offset /*= - 1*/, int count /*= 0*/)
{
    return zrangeby("ZREVRANGEBYSCORE", key, TO_STRING(min),
                    TO_STRING(max), result, offset, count);
}

int redis_zset::zrevrangebyscore(const std::string& key, const std::string& min,
    const std::string& max, std::vector<std::string>& result,
    int offset /*= -1*/, int count /*= 0*/)
{
    return zrangeby("ZREVRANGEBYSCORE", key, min, max, result, offset, count);
}

int redis_zset::zrevrangebyscore_with_scores(const std::string& key,
    double min, double max,
    std::vector< std::pair<std::string, double> >& result,
    int offset /*= -1*/, int count /*= 0*/)
{
    std::vector<std::string> elem_score;
    int iret = zrangeby("ZREVRANGEBYSCORE", key, TO_STRING(min),
                        TO_STRING(max),elem_score, offset, count, true);

    return (iret > 0) ? parse_element_score(elem_score, result) : iret;
}

int redis_zset::zrevrangebyscore_with_scores(const std::string& key,
    const std::string& min, const std::string& max,
    std::vector< std::pair<std::string, double> >& result,
    int offset /*= -1*/, int count /*= 0*/)
{
    std::vector<std::string> elem_score;
    int iret = zrangeby("ZREVRANGEBYSCORE", key, min, max,
                         elem_score, offset, count, true);

    return (iret > 0) ? parse_element_score(elem_score, result) : iret;
}

int redis_zset::zrevrank(const std::string& key, const std::string& member)
{
    std::vector<std::string> argv;
    argv.push_back("ZREVRANK");
    argv.push_back(key.c_str());
    argv.push_back(member.c_str());

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

int redis_zset::zscan(const std::string& key, int cursor,
                      std::vector< std::pair<std::string, double> >& result,
                      const char* pattern /*= NULL*/, int count /*= 10*/)
{
    scan_keys("ZSCAN", &key, cursor, pattern, count);

    std::vector<std::string> key_val;
    cursor = get_cursor_array(&key_val);
    parse_element_score(key_val, result);

    return cursor;
}

int redis_zset::zscore(const std::string& key, const std::string& member,
                       std::string& score)
{
    std::vector<std::string> argv;
    argv.push_back("ZSCORE");
    argv.push_back(key.c_str());
    argv.push_back(member.c_str());

    build_request(argv);
    hash_slots(key);

    return get_string_or_nil(score);
}

