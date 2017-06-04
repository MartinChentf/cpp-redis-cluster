#include "RedisClient.h"

RedisClient::RedisClient(const std::string & host, uint16_t port)
: redis_command(host, port)
, redis_connection(host, port)
, redis_hash(host, port)
, redis_key(host, port)
, redis_list(host, port)
, redis_server(host, port)
, redis_set(host, port)
, redis_string(host, port)
, redis_zset(host, port)
{
}

