#include "redis_key.h"
#include "Util.h"

redis_key::redis_key(const std::string & host, uint16_t port)
: redis_command(host, port)
{
}

int redis_key::del(const std::vector<std::string>& vkeys)
{
    std::vector<std::string> argv;
    argv.push_back("DEL");

    for (size_t i = 0; i < vkeys.size(); i++) {
        argv.push_back(vkeys[i]);
    }

    sendCommand(argv);

    return get_integer32();
}

int redis_key::del(const std::string& key)
{
    std::vector<std::string> vkeys;
    vkeys.push_back(key);

    return del(vkeys);
}

int redis_key::dump(const std::string& key, std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("DUMP");
    argv.push_back(key);

    sendCommand(argv);

    return get_string_or_nil(result);
}

int redis_key::exists(const std::vector<std::string>& vkeys)
{
    std::vector<std::string> argv;
    argv.push_back("EXISTS");

    for (size_t i = 0; i < vkeys.size(); i++) {
        argv.push_back(vkeys[i]);
    }

    sendCommand(argv);

    return get_integer32();
}

int redis_key::exists(const std::string& key)
{
    std::vector<std::string> argv;
    argv.push_back("EXISTS");
    argv.push_back(key);

    sendCommand(argv);

    return get_integer32();
}

int redis_key::expire(const std::string& key, int second)
{
    std::vector<std::string> argv;
    argv.push_back("EXPIRE");
    argv.push_back(key);
    argv.push_back(TO_STRING(second));

    sendCommand(argv);

    return get_integer32();
}

int redis_key::expireat(const std::string& key, time_t timestamp)
{
    std::vector<std::string> argv;
    argv.push_back("EXPIREAT");
    argv.push_back(key);
    argv.push_back(TO_STRING(timestamp));

    sendCommand(argv);

    return get_integer32();
}

int redis_key::keys(const std::string& pattern,std::vector<std::string>& result)
{
    std::vector<std::string> argv;
    argv.push_back("KEYS");
    argv.push_back(pattern);

    sendCommand(argv);

    return get_array_or_nil(result);
}

bool redis_key::migrate(const std::string& host, uint16_t port,
    const std::string& key, int dest_db, long long timeout,
    MigrateParams* params /*= NULL*/)
{
    std::vector<std::string> argv;
    argv.push_back("MIGRATE");
    argv.push_back(host);
    argv.push_back(TO_STRING(port));    
    argv.push_back(key);    
    argv.push_back(TO_STRING(dest_db));
    argv.push_back(TO_STRING(timeout));

    if (params != NULL) {
        argv.insert(argv.end(), params->getParams().begin(),
            params->getParams().end());
    }

    sendCommand(argv);

    return check_status();
}

int redis_key::move(const std::string& key, int db)
{
    std::vector<std::string> argv;
    argv.push_back("MOVE");
    argv.push_back(key);    
    argv.push_back(TO_STRING(db));

    sendCommand(argv);

    return get_integer32();
}

int redis_key::object_refcount(const std::string& key)
{
    std::vector<std::string> argv;
    argv.push_back("OBJECT");
    argv.push_back("REFCOUNT");
    argv.push_back(key);    

    sendCommand(argv);

    return get_integer32_or_nil();
}

bool redis_key::object_encoding(const std::string& key, std::string& encode)
{
    std::vector<std::string> argv;
    argv.push_back("OBJECT");
    argv.push_back("ENCODING");
    argv.push_back(key);    

    sendCommand(argv);

    return get_string_or_nil(encode) >= 0;
}

long long redis_key::object_idletime(const std::string& key)
{
    std::vector<std::string> argv;
    argv.push_back("OBJECT");
    argv.push_back("IDLETIME");
    argv.push_back(key);    

    sendCommand(argv);

    return get_integer64_or_nil();
}

int redis_key::persist(const std::string& key)
{
    std::vector<std::string> argv;
    argv.push_back("PERSIST");
    argv.push_back(key);    

    sendCommand(argv);

    return get_integer32();
}

int redis_key::pexpire(const std::string& key, long long milliseconds)
{
    std::vector<std::string> argv;
    argv.push_back("PEXPIRE");
    argv.push_back(key);
    argv.push_back(TO_STRING(milliseconds));

    sendCommand(argv);

    return get_integer32();
}

int redis_key::pexpireat(const std::string& key,
                         unsigned long long milliseconds_timestamp)
{
    std::vector<std::string> argv;
    argv.push_back("PEXPIREAT");
    argv.push_back(key);
    argv.push_back(TO_STRING(milliseconds_timestamp));

    sendCommand(argv);

    return get_integer32();
}

long long redis_key::pttl(const std::string & key)
{
    std::vector<std::string> argv;
    argv.push_back("PTTL");
    argv.push_back(key);

    sendCommand(argv);

    return get_integer64();
}

int redis_key::randomkey(std::string & out)
{
    std::vector<std::string> argv;
    argv.push_back("RANDOMKEY");

    sendCommand(argv);

    return get_string_or_nil(out);
}

bool redis_key::rename(const std::string& key, const std::string& new_key)
{
    std::vector<std::string> argv;
    argv.push_back("RENAME");
    argv.push_back(key);
    argv.push_back(new_key);

    sendCommand(argv);

    return check_status();
}

int redis_key::renamenx(const std::string& key, const std::string& new_key)
{
    std::vector<std::string> argv;
    argv.push_back("RENAMENX");
    argv.push_back(key);
    argv.push_back(new_key);

    sendCommand(argv);

    return get_integer32();
}

bool redis_key::restore(const std::string& key, unsigned long long TTL,
    const std::string& serialized_value, bool is_replace /*= false*/)
{
    std::vector<std::string> argv;
    argv.push_back("RESTORE");
    argv.push_back(key);
    argv.push_back(TO_STRING(TTL));
    argv.push_back(serialized_value);

    if (is_replace) {
        argv.push_back("REPLACE");
    }

    sendCommand(argv);

    return check_status();
}

int redis_key::scan(int cursor, std::vector<std::string>& result,
                    const char * pattern /*= NULL*/, int count /*= 10*/)
{
    scan_keys("SCAN", NULL, cursor, pattern, count);
    return get_cursor_array(&result);
}

int redis_key::sort(const std::string& key, const std::string& dest,
                    SortParams* params /*= NULL*/)
{
    std::vector<std::string> argv;
    argv.push_back("SORT");
    argv.push_back(key);
    if (params) {
        argv.insert(argv.end(), params->getParams().begin(),
            params->getParams().end());
    }
    argv.push_back("STORE");
    argv.push_back(dest);

    sendCommand(argv);

    return get_integer32();
}

int redis_key::sort(const std::string& key, std::vector<std::string>& result,
                    SortParams* params /*= NULL*/)
{
    std::vector<std::string> argv;
    argv.push_back("SORT");
    argv.push_back(key);
    if (params) {
        argv.insert(argv.end(), params->getParams().begin(),
            params->getParams().end());
    }

    sendCommand(argv);

    return get_array_or_nil(result);
}

int redis_key::touch(const std::vector<std::string>& vkeys)
{
    std::vector<std::string> argv;
    argv.push_back("TOUCH");

    for (size_t i = 0; i < vkeys.size(); i++) {
        argv.push_back(vkeys[i]);
    }

    sendCommand(argv);

    return get_integer32();
}

int redis_key::touch(const std::string& key)
{
    std::vector<std::string> vkeys;
    vkeys.push_back(key);

    return touch(vkeys);
}

long long redis_key::ttl(const std::string & key)
{
    std::vector<std::string> argv;
    argv.push_back("TTL");
    argv.push_back(key);

    sendCommand(argv);

    return get_integer64();
}

std::string redis_key::type(const std::string & key)
{
    std::vector<std::string> argv;
    argv.push_back("TYPE");
    argv.push_back(key);

    sendCommand(argv);

    return get_status();
}

/**
 * sort_params类定义
 */
SortParams& SortParams::by(const std::string& pattern)
{
    params.push_back("BY");
    params.push_back(pattern);
    return *this;
}

SortParams& SortParams::nosort()
{
    params.push_back("BY");
    params.push_back("NOSORT");
    return *this;
}

SortParams& SortParams::get(const std::string& pattern)
{
    params.push_back("GET");
    params.push_back(pattern);
    return *this;
}

SortParams& SortParams::limit(int offset, int count)
{
    params.push_back("LIMIT");
    params.push_back(TO_STRING(offset));
    params.push_back(TO_STRING(count));
    return *this;
}

SortParams& SortParams::asc()
{
    params.push_back("ASC");
    return *this;
}

SortParams& SortParams::desc()
{
    params.push_back("DESC");
    return *this;
}

SortParams& SortParams::alpha()
{
    params.push_back("ALPHA");
    return *this;
}

/**
 * MigrateParams 类定义
 */
MigrateParams& MigrateParams::copy()
{
    params.push_back("COPY");
    return *this;
}

MigrateParams& MigrateParams::replace()
{
    params.push_back("REPLACE");
    return *this;
}

MigrateParams& MigrateParams::keys(const std::vector<std::string>& vkeys)
{
    params.push_back("KEYS");
    params.insert(params.end(), vkeys.begin(), vkeys.end());
    return *this;
}
