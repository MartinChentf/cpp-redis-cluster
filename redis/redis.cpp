#include "redis.h"

redis::redis(const std::string host, uint16_t port)
: m_client(NULL)
{
    m_client = new RedisClient(host, port);
}

redis::~redis()
{
    if (m_client) {
        delete m_client;
        m_client = NULL;
    }
}


/***************************
 *  string command
 ***************************/
int redis::get(const std::string& key, std::string& result)
{
    return m_client->get(key, result);
}

int redis::getSet(const std::string& key, const std::string& value,
    std::string& result)
{
    return m_client->getSet(key, value, result);
}

bool redis::set(const std::string& key, const std::string& value,
    SET_PARAM ex_px, long long timeout, SET_PARAM nx_xx)
{
    return m_client->set(key, value, ex_px, timeout, nx_xx);
}

bool redis::set(const std::string& key, const std::string& value)
{
    return m_client->set(key, value);
}

bool redis::setnx(const std::string& key, const std::string& value)
{
    return m_client->setnx(key, value);
}

bool redis::setxx(const std::string& key, const std::string& value)
{
    return m_client->setxx(key, value);
}

bool redis::setex(const std::string& key, long long second,
    const std::string& value)
{
    return m_client->setex(key, second, value);
}

bool redis::psetex(const std::string& key, long long millisecond,
    const std::string& value)
{
    return m_client->psetex(key, millisecond, value);
}

bool redis::getrange(const std::string& key, int start, int end,
    std::string& result)
{
    return m_client->getrange(key, start, end, result);
}

bool redis::setrange(const std::string& key, int offset,
    const std::string& value, long long* length /*= NULL*/)
{
    return m_client->setrange(key, offset, value, length);
}

int redis::getbit(const std::string& key, int offset)
{
    return m_client->getbit(key, offset);
}

int redis::setbit(const std::string& key, int offset, bool value)
{
    return m_client->setbit(key, offset, value);
}

long long redis::bitcount(const std::string& key)
{
    return m_client->bitcount(key);
}

long long redis::bitcount(const std::string& key, int start, int end)
{
    return m_client->bitcount(key, start, end);
}

long long redis::bitop(BITOP op, const std::string& dest_key,
    const std::vector<std::string>& src_keys)
{
    return m_client->bitop(op, dest_key, src_keys);
}

long long redis::bitop(BITOP op, const std::string& dest_key,
    const std::string& src_key)
{
    return m_client->bitop(op, dest_key, src_key);
}

long long redis::bitpos(const std::string& key, bool value,
    int start /*= 0*/, int end /*= -1*/)
{
    return m_client->bitpos(key, value, start, end);
}

bool redis::mget(const std::vector<std::string>& vkeys,
    std::vector<std::string*>& result)
{
    return m_client->mget(vkeys, result);
}

bool redis::mset(const std::map<std::string, std::string>& key_values)
{
    return m_client->mset(key_values);
}

bool redis::msetnx(const std::map<std::string, std::string>& key_values)
{
    return m_client->msetnx(key_values);
}

bool redis::incr(const std::string& key, long long* result /*= NULL*/)
{
    return m_client->incr(key, result);
}

bool redis::incrby(const std::string& key, long long increment,
    long long* result /*= NULL*/)
{
    return m_client->incrby(key, increment, result);
}

bool redis::incrbyfloat(const std::string& key, double increment,
    std::string* result /*= NULL*/)
{
    return m_client->incrbyfloat(key, increment, result);
}

bool redis::decr(const std::string& key, long long* result /*= NULL*/)
{
    return m_client->decr(key, result);
}

bool redis::decrby(const std::string& key, long long decrement,
    long long* result /*= NULL*/)
{
    return m_client->decrby(key, decrement, result);
}

bool redis::append(const std::string& key, const std::string& value,
    long long* length /*= NULL*/)
{
    return m_client->append(key, value, length);
}

bool redis::strlen(const std::string& key, long long& length)
{
    return m_client->strlen(key, length);
}


/***************************
 *  list command
 ***************************/
int redis::blpop(const std::vector<std::string>& vkeys, long long timeout,
    std::string& key, std::string& value)
{
    return m_client->blpop(vkeys, timeout, key, value);
}

int redis::brpop(const std::vector<std::string>& vkeys, long long timeout,
    std::string& key, std::string& value)
{
    return m_client->brpop(vkeys, timeout, key, value);
}

int redis::brpoplpush(const std::string& src, const std::string& dest,
    long long timeout, std::string& result)
{
    return m_client->brpoplpush(src, dest, timeout, result);
}

int redis::lindex(const std::string& key, int index, std::string& result)
{
    return m_client->lindex(key, index, result);
}

long long redis::linsert(const std::string& key, bool is_before,
    const std::string& pivot, const std::string& value)
{
    return m_client->linsert(key, is_before, pivot, value);
}

long long redis::llen(const std::string& key)
{
    return m_client->llen(key);
}

int redis::lpop(const std::string& key, std::string& result)
{
    return m_client->lpop(key, result);
}

long long redis::lpush(const std::string& key,
    const std::vector<std::string>& values)
{
    return m_client->lpush(key, values);
}

long long redis::lpushx(const std::string& key, const std::string& value)
{
    return m_client->lpushx(key, value);
}

bool redis::lrange(const std::string& key, int start, int stop,
    std::vector<std::string>& result)
{
    return m_client->lrange(key, start, stop, result);
}

long long redis::lrem(const std::string& key, int count,
    const std::string& value)
{
    return m_client->lrem(key, count, value);
}

bool redis::lset(const std::string& key, int index, const std::string& value)
{
    return m_client->lset(key, index, value);
}

bool redis::ltrim(const std::string& key, int start, int stop)
{
    return m_client->ltrim(key, start, stop);
}

int redis::rpop(const std::string& key, std::string& result)
{
    return m_client->rpop(key, result);
}

int redis::rpoplpush(const std::string& src, const std::string& dest,
    std::string& result)
{
    return m_client->rpoplpush(src, dest, result);
}

long long redis::rpush(const std::string& key,
    const std::vector<std::string>& values)
{
    return m_client->rpush(key, values);
}

long long redis::rpushx(const std::string& key, const std::string& value)
{
    return m_client->rpushx(key, value);
}


/***************************
 *  hash command
 ***************************/
long long redis::hdel(const std::string& key,
    const std::vector<std::string>& fields)
{
    return m_client->hdel(key, fields);
}

int redis::hexists(const std::string& key, const std::string& field)
{
    return m_client->hexists(key, field);
}

int redis::hget(const std::string& key, const std::string& field,
    std::string& result)
{
    return m_client->hget(key, field, result);
}

int redis::hgetall(const std::string& key, std::map<std::string,
    std::string>& result)
{
    return m_client->hgetall(key, result);
}

bool redis::hincrby(const std::string& key, const std::string& field,
    long long increment, long long* result /*= NULL*/)
{
    return m_client->hincrby(key, field, increment, result);
}

bool redis::hincrbyfloat(const std::string& key, const std::string& field,
    double increment, std::string* result /*= NULL*/)
{
    return m_client->hincrbyfloat(key, field, increment, result);
}

bool redis::hkeys(const std::string& key, std::vector<std::string>& result)
{
    return m_client->hkeys(key, result);
}

long long redis::hlen(const std::string& key)
{
    return m_client->hlen(key);
}

bool redis::hmget(const std::string& key,
    const std::vector<std::string>& fields,
    std::vector<std::string*>& result)
{
    return m_client->hmget(key, fields, result);
}

bool redis::hmset(const std::string& key,
    const std::map<std::string, std::string>& field_values)
{
    return m_client->hmset(key, field_values);
}

int redis::hscan(const std::string& key, int cursor,
    std::map<std::string, std::string>& result,
    const char* pattern /*= NULL*/, int count /*= 10*/)
{
    return m_client->hscan(key, cursor, result, pattern, count);
}

int redis::hset(const std::string& key, const std::string& field,
    const std::string& value)
{
    return m_client->hset( key, field, value);
}

int redis::hsetnx(const std::string& key, const std::string& field,
    const std::string& value)
{
    return m_client->hsetnx(key, field, value);
}

long long redis::hstrlen(const std::string& key, const std::string& field)
{
    return m_client->hstrlen(key, field);
}

bool redis::hvals(const std::string& key, std::vector<std::string>& result)
{
    return m_client->hvals(key, result);
}


/***************************
 *  set command
 ***************************/
long long redis::sadd(const std::string& key,
    const std::vector<std::string>& member)
{
    return m_client->sadd(key, member);
}

long long redis::scard(const std::string& key)
{
    return m_client->scard(key);
}

bool redis::sdiff(const std::vector<std::string>& vkeys,
    std::vector<std::string>& result)
{
    return m_client->sdiff(vkeys, result);
}

bool redis::sinter(const std::vector<std::string>& vkeys,
    std::vector<std::string>& result)
{
    return m_client->sinter(vkeys, result);
}

bool redis::sunion(const std::vector<std::string>& vkeys,
    std::vector<std::string>& result)
{
    return m_client->sunion(vkeys, result);
}

long long redis::sdiffstore(const std::string& dest,
    const std::vector<std::string>& vkeys)
{
    return m_client->sdiffstore(dest, vkeys);
}

long long redis::sinterstore(const std::string& dest,
    const std::vector<std::string>& vkeys)
{
    return m_client->sinterstore(dest, vkeys);
}

long long redis::sunionstore(const std::string& dest,
    const std::vector<std::string>& vkeys)
{
    return m_client->sunionstore(dest, vkeys);
}

int redis::sismember(const std::string& key,
    const std::string& member)
{
    return m_client->sismember(key, member);
}

int redis::smembers(const std::string& key,
    std::vector<std::string>& result)
{
    return m_client->smembers(key, result);
}

int redis::smove(const std::string& src, const std::string& dest,
    const std::string& member)
{
    return m_client->smove(src, dest, member);
}

int redis::spop(const std::string& key, std::string& result)
{
    return m_client->spop(key, result);
}

int redis::spop(const std::string& key, std::vector<std::string>& result,
    int count)
{
    return m_client->spop(key, result, count);
}

int redis::srandmember(const std::string& key, std::string& result)
{
    return m_client->srandmember(key, result);
}

int redis::srandmember(const std::string& key,
    std::vector<std::string>& result, int count)
{
    return m_client->srandmember(key, result, count);
}

long long redis::srem(const std::string& key,
    const std::vector<std::string>& member)
{
    return m_client->srem(key, member);
}

int redis::sscan(const std::string& key, int cursor,
    std::vector<std::string>& result,
    const char* pattern /*= NULL*/, int count /*= 10*/)
{
    return m_client->sscan(key, cursor, result, pattern, count);
}


/***************************
 *  zset command
 ***************************/
long long redis::zadd(const std::string& key,
    const std::vector< std::pair<std::string, double> >& member_score,
    int nx_or_xx /*= 0*/, bool is_change /*= false*/)
{
    return m_client->zadd(key, member_score, nx_or_xx, is_change);
}

long long redis::zadd_nx(const std::string& key,
    const std::vector< std::pair<std::string, double> >& member_score,
    bool is_change /*= false*/)
{
    return m_client->zadd_nx(key, member_score, is_change);
}
    
long long redis::zadd_xx(const std::string& key,
    const std::vector< std::pair<std::string, double> >& member_score,
    bool is_change /*= false*/)
{
    return m_client->zadd_xx(key, member_score, is_change);
}
    
long long redis::zadd_ch(const std::string& key,
    const std::vector< std::pair<std::string, double> >& member_score,
    int nx_or_xx /*= 0*/)
{
    return m_client->zadd_ch(key, member_score, nx_or_xx);
}
    
bool redis::zadd_incr(const std::string& key, double score,
    const std::string& member, std::string& result)
{
    return m_client->zadd_incr(key, score, member, result);
}

long long redis::zcard(const std::string& key)
{
    return m_client->zcard(key);
}

long long redis::zcount(const std::string& key, double min, double max)
{
    return m_client->zcount(key, min, max);
}

long long redis::zcount(const std::string& key, const std::string& min,
    const std::string& max)
{
    return m_client->zcount(key, min, max);
}

bool redis::zincrby(const std::string& key, double increment,
    const std::string& member, std::string* result /*= NULL*/)
{
    return m_client->zincrby(key, increment, member, result);
}

long long redis::zinterstore(const std::string& dest,
    const std::vector<std::string>& vkeys,
    const std::vector<double>* weights /*= NULL*/,
    const char* aggregate /*= "SUM"*/)
{
    return m_client->zinterstore(dest, vkeys, weights, aggregate);
}

long long redis::zunionstore(const std::string& dest,
    const std::vector<std::string>& vkeys,
    const std::vector<double>* weights /*= NULL*/,
    const char* aggregate /*= "SUM"*/)
{
    return m_client->zunionstore(dest, vkeys, weights, aggregate);
}

long long redis::zlexcount(const std::string& key, const std::string& min,
    const std::string& max)
{
    return m_client->zlexcount(key, min, max);
}

int redis::zrange(const std::string& key, int start, int stop,
    std::vector<std::string>& result)
{
    return m_client->zrange(key, start, stop, result);
}

int redis::zrange_with_scores(const std::string& key, int start, int stop,
    std::vector< std::pair<std::string, double> >& result)
{
    return m_client->zrange_with_scores(key, start, stop, result);
}

int redis::zrangebylex(const std::string& key, const std::string& min,
    const std::string& max, std::vector<std::string>& result,
    int offset /*= -1*/, int count /*= 0*/)
{
    return m_client->zrangebylex(key, min, max, result, offset, count);
}

int redis::zrangebyscore(const std::string& key, double min, double max,
    std::vector<std::string>& result, int offset /*= -1*/, int count /*= 0*/)
{
    return m_client->zrangebyscore(key, min, max, result, offset, count);
}

int redis::zrangebyscore(const std::string& key, const std::string& min,
    const std::string& max, std::vector<std::string>& result,
    int offset /*= -1*/, int count /*= 0*/)
{
    return m_client->zrangebyscore(key, min, max, result, offset, count);
}

int redis::zrangebyscore_with_scores(const std::string& key,
    double min, double max,
    std::vector< std::pair<std::string, double> >& result,
    int offset /*= -1*/, int count /*= 0*/)
{
    return m_client->zrangebyscore_with_scores(key, min, max, result, offset, count);
}

int redis::zrangebyscore_with_scores(const std::string& key,
    const std::string& min, const std::string& max,
    std::vector< std::pair<std::string, double> >& result,
    int offset /*= -1*/, int count /*= 0*/)
{
    return m_client->zrangebyscore_with_scores(key, min, max, result, offset, count);
}
    
int redis::zrank(const std::string& key, const std::string& member)
{
    return m_client->zrank(key, member);
}

long long redis::zrem(const std::string& key,
    const std::vector<std::string>& member)
{
    return m_client->zrem(key, member);
}

int redis::zremrangebylex(const std::string& key, const std::string& min,
    const std::string& max)
{
    return m_client->zremrangebylex(key, min, max);
}

int redis::zremrangebyrank(const std::string& key, int start, int stop)
{
    return m_client->zremrangebyrank(key, start, stop);
}

int redis::zremrangebyscore(const std::string& key, double min, double max)
{
    return m_client->zremrangebyscore(key, min, max);
}

int redis::zremrangebyscore(const std::string& key, const std::string& min,
    const std::string& max)
{
    return m_client->zremrangebyscore(key, min, max);
}

int redis::zrevrange(const std::string& key, int start, int stop,
              std::vector<std::string>& result)
{
    return m_client->zrevrange(key, start, stop, result);
}

int redis::zrevrange_with_scores(const std::string& key,
    int start, int stop,
    std::vector< std::pair<std::string, double> >& result)
{
    return m_client->zrevrange_with_scores(key, start, stop, result);
}

int redis::zrevrangebylex(const std::string& key, const std::string& min,
    const std::string& max, std::vector<std::string>& result,
    int offset /*= -1*/, int count /*= 0*/)
{
    return m_client->zrevrangebylex(key, min, max, result, offset, count);
}

int redis::zrevrangebyscore(const std::string& key, double min, double max,
    std::vector<std::string>& result, int offset /*= -1*/, int count /*= 0*/)
{
    return m_client->zrevrangebyscore(key, min, max, result, offset, count);
}

int redis::zrevrangebyscore(const std::string& key, const std::string& min,
    const std::string& max, std::vector<std::string>& result,
    int offset /*= -1*/, int count /*= 0*/)
{
    return m_client->zrevrangebyscore(key, min, max, result, offset, count);
}

int redis::zrevrangebyscore_with_scores(const std::string& key,
    double min, double max, std::vector< std::pair<std::string, double> >& result,
    int offset /*= -1*/, int count /*= 0*/)
{
    return m_client->zrevrangebyscore_with_scores(key, min, max, result, offset, count);
}

int redis::zrevrangebyscore_with_scores(const std::string& key,
    const std::string& min, const std::string& max,
    std::vector< std::pair<std::string, double> >& result,
    int offset /*= -1*/, int count /*= 0*/)
{
    return m_client->zrevrangebyscore_with_scores(key, min, max, result, offset, count);
}

int redis::zrevrank(const std::string& key, const std::string& member)
{
    return m_client->zrevrank(key, member);
}

int redis::zscan(const std::string& key, int cursor,
    std::vector< std::pair<std::string, double> >& result,
    const char* pattern /*= NULL*/, int count /*= 10*/)
{
    return m_client->zscan(key, cursor, result, pattern, count);
}

int redis::zscore(const std::string& key, const std::string& member,
    std::string& score)
{
    return m_client->zscore(key, member, score);
}


/***************************
 *  keys command
 ***************************/
int redis::del(const std::vector<std::string>& vkeys)
{
    return m_client->del(vkeys);
}

int redis::del(const std::string& key)
{
    return m_client->del(key);
}

int redis::dump(const std::string& key, std::string& result)
{
    return m_client->dump(key, result);
}

int redis::exists(const std::vector<std::string>& vkeys)
{
    return m_client->exists(vkeys);
}

int redis::exists(const std::string& key)
{
    return m_client->exists(key);
}

int redis::expire(const std::string& key, int second)
{
    return m_client->expire(key, second);
}

int redis::expireat(const std::string& key, time_t timestamp)
{
    return m_client->expireat(key, timestamp);
}

int redis::keys(const std::string& pattern, std::vector<std::string>& result)
{
    return m_client->keys(pattern, result);
}

bool redis::migrate(const std::string& host, uint16_t port,
    const std::string& key, int dest_db, long long timeout,
    MigrateParams * params /*= NULL*/)
{
    return m_client->migrate(host, port, key, dest_db, timeout, params);
}

int redis::move(const std::string& key, int db)
{
    return m_client->move(key, db);
}

int redis::object_refcount(const std::string& key)
{
    return m_client->object_refcount(key);
}

bool redis::object_encoding(const std::string& key, std::string& encode)
{
    return m_client->object_encoding(key, encode);
}

long long redis::object_idletime(const std::string& key)
{
    return m_client->object_idletime(key);
}

int redis::persist(const std::string& key)
{
    return m_client->persist(key);
}

int redis::pexpire(const std::string& key, long long milliseconds)
{
    return m_client->pexpire(key, milliseconds);
}

int redis::pexpireat(const std::string& key,
              unsigned long long milliseconds_timestamp)
{
    return m_client->pexpireat(key, milliseconds_timestamp);
}

long long redis::pttl(const std::string& key)
{
    return m_client->pttl(key);
}

int redis::randomkey(std::string& out)
{
    return m_client->randomkey(out);
}

bool redis::rename(const std::string& key, const std::string& new_key)
{
    return m_client->rename(key, new_key);
}

int redis::renamenx(const std::string& key, const std::string& new_key)
{
    return m_client->renamenx(key, new_key);
}

bool redis::restore(const std::string& key, unsigned long long TTL,
             const std::string& serialized_value, bool is_replace /*= false*/)
{
    return m_client->restore(key, TTL, serialized_value, is_replace);
}

int redis::scan(int cursor, std::vector<std::string>& result,
         const char* pattern /*= NULL*/, int count /*= 10*/)
{
    return m_client->scan(cursor, result, pattern, count);
}

int redis::sort(const std::string& key, const std::string& dest,
         SortParams* params /*= NULL*/)
{
    return m_client->sort(key, dest, params);
}

int redis::sort(const std::string& key, std::vector<std::string>& result,
         SortParams* params /*= NULL*/)
{
    return m_client->sort(key, result, params);
}

int redis::touch(const std::vector<std::string>& vkeys)
{
    return m_client->touch(vkeys);
}

int redis::touch(const std::string& key)
{
    return m_client->touch(key);
}

long long redis::ttl(const std::string& key)
{
    return m_client->ttl(key);
}

std::string redis::type(const std::string& key)
{
    return m_client->type(key);
}


/***************************
 *  server command
 ***************************/
bool redis::bgrewriteaof()
{
    return m_client->bgrewriteaof();
}

std::string redis::bgsave()
{
    return m_client->bgsave();
}

bool redis::clientGetname(std::string & name)
{
    return m_client->clientGetname(name);
}

bool redis::clientKill(const std::string & addr)
{
    return m_client->clientKill(addr);
}

int redis::clientKill(KillFilterParams params)
{
    return m_client->clientKill(params);
}

std::string redis::clientList(
    std::vector<std::map<std::string,std::string>*>* list /*= NULL*/)
{
    return m_client->clientList(list);
}

bool redis::clientPause(long long timeout)
{
    return m_client->clientPause(timeout);
}

int redis::clientReply(CLIENT_REPLY reply)
{
    return m_client->clientReply(reply);
}

bool redis::clientSetname(const std::string & connName)
{
    return m_client->clientSetname(connName);
}

bool redis::flushAll()
{
    return m_client->flushAll();
}

bool redis::flushDB()
{
    return m_client->flushDB();
}

long long redis::lastsave()
{
    return m_client->lastsave();
}

bool redis::save()
{
    return m_client->save();
}

/***************************
 *  connection command
 ***************************/
bool redis::auth(const std::string password)
{
    return m_client->auth(password);
}

std::string redis::echo(const std::string message)
{
    return m_client->echo(message);
}

std::string redis::ping(const std::string* message)
{
    return m_client->ping(message);
}

bool redis::ping()
{
    return m_client->ping();
}

bool redis::quit()
{
    return m_client->quit();
}

bool redis::select(int index)
{
    return m_client->select(index);
}
