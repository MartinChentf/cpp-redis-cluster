#include "redis_key.h"
#include "redis_helper.h"

int redis_key::del(const std::vector<std::string>& keys)
{
    std::vector<std::string> argv;
    argv.push_back("DEL");

    for (size_t i = 0; i < keys.size(); i++) {
        argv.push_back(keys[i]);
    }

    build_request(argv);

    if (!keys.empty()) {
        hash_slots(keys[0]);
    }

    return get_integer32();
}

int redis_key::del(const std::string& key)
{
    std::vector<std::string> keys;
    keys.push_back(key);

    return del(keys);
}

int redis_key::dump(const std::string& key, std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("DUMP");
    argv.push_back(key);

    build_request(argv);
    hash_slots(key);

    return get_string_or_nil(result);
}

int redis_key::exists(const std::vector<std::string>& keys)
{
    std::vector<std::string> argv;
    argv.push_back("EXISTS");

    for (size_t i = 0; i < keys.size(); i++) {
        argv.push_back(keys[i]);
    }

    build_request(argv);
    if (!keys.empty()) {
        hash_slots(keys[0]);
    }

    return get_integer32();
}

int redis_key::exists(const std::string& key)
{
    std::vector<std::string> argv;
    argv.push_back("EXISTS");
    argv.push_back(key);

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

int redis_key::expire(const std::string& key, int second)
{
    std::vector<std::string> argv;
    argv.push_back("EXPIRE");
    argv.push_back(key);
    argv.push_back(TO_STRING(second));

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

int redis_key::expireat(const std::string& key, time_t timestamp)
{
    std::vector<std::string> argv;
    argv.push_back("EXPIREAT");
    argv.push_back(key);
    argv.push_back(TO_STRING(timestamp));

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

int redis_key::keys(const std::string& pattern,std::vector<std::string>& result)
{
    std::vector<std::string> argv;
    argv.push_back("KEYS");
    argv.push_back(pattern);

    build_request(argv);

    return get_array_or_nil(result);
}

bool redis_key::migrate(const std::string& host, uint16_t port,
    const std::string& key, int dest_db, long long timeout,
    bool is_copy /*= false*/, bool is_replace /*= false*/,
    const std::vector<std::string>* keys /*= NULL*/)
{
    std::vector<std::string> argv;
    argv.push_back("MIGRATE");
    argv.push_back(host);
    argv.push_back(TO_STRING(port));    
    argv.push_back(key);    
    argv.push_back(TO_STRING(dest_db));
    argv.push_back(TO_STRING(timeout));

    if (is_copy) {
        argv.push_back("COPY");
    }
    if (is_replace) {
        argv.push_back("REPLACE");
    }
    if (key == "" && keys != NULL) {
        argv.push_back("KEYS");
        for (size_t i = 0; i < keys->size(); i++) {
            argv.push_back((*keys)[i]);
        }
    }

    build_request(argv);

    return check_status();
}

int redis_key::move(const std::string& key, int db)
{
    std::vector<std::string> argv;
    argv.push_back("MOVE");
    argv.push_back(key);    
    argv.push_back(TO_STRING(db));

    build_request(argv);

    return get_integer32();
}

int redis_key::object_refcount(const std::string& key)
{
    std::vector<std::string> argv;
    argv.push_back("OBJECT");
    argv.push_back("REFCOUNT");
    argv.push_back(key);    

    build_request(argv);

    return get_integer32_or_nil();
}

bool redis_key::object_encoding(const std::string& key, std::string& encode)
{
    std::vector<std::string> argv;
    argv.push_back("OBJECT");
    argv.push_back("ENCODING");
    argv.push_back(key);    

    build_request(argv);

    return get_string_or_nil(encode) >= 0;
}

long long redis_key::object_idletime(const std::string& key)
{
    std::vector<std::string> argv;
    argv.push_back("OBJECT");
    argv.push_back("IDLETIME");
    argv.push_back(key);    

    build_request(argv);

    return get_integer64_or_nil();
}

int redis_key::persist(const std::string& key)
{
    std::vector<std::string> argv;
    argv.push_back("PERSIST");
    argv.push_back(key);    

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

int redis_key::pexpire(const std::string& key, long long milliseconds)
{
    std::vector<std::string> argv;
    argv.push_back("PEXPIRE");
    argv.push_back(key);
    argv.push_back(TO_STRING(milliseconds));

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

int redis_key::pexpireat(const std::string& key,
                         unsigned long long milliseconds_timestamp)
{
    std::vector<std::string> argv;
    argv.push_back("PEXPIREAT");
    argv.push_back(key);
    argv.push_back(TO_STRING(milliseconds_timestamp));

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

long long redis_key::pttl(const std::string & key)
{
    std::vector<std::string> argv;
    argv.push_back("PTTL");
    argv.push_back(key);

    build_request(argv);
    hash_slots(key);

    return get_integer64();
}

int redis_key::randomkey(std::string & out)
{
    std::vector<std::string> argv;
    argv.push_back("RANDOMKEY");

    build_request(argv);

    return get_string_or_nil(out);
}

bool redis_key::rename(const std::string& key, const std::string& new_key)
{
    std::vector<std::string> argv;
    argv.push_back("RENAME");
    argv.push_back(key);
    argv.push_back(new_key);

    build_request(argv);
    hash_slots(key);

    return check_status();
}

bool redis_key::renamenx(const std::string& key, const std::string& new_key)
{
    std::vector<std::string> argv;
    argv.push_back("RENAMENX");
    argv.push_back(key);
    argv.push_back(new_key);

    build_request(argv);
    hash_slots(key);

    return check_status();
}

bool redis_key::restore(const std::string& key, unsigned long long ttl,
    const std::string& serialized_value, bool is_replace /*= false*/)
{
    std::vector<std::string> argv;
    argv.push_back("RESTORE");
    argv.push_back(key);
    argv.push_back(TO_STRING(ttl));
    argv.push_back(serialized_value);

    if (is_replace) {
        argv.push_back("REPLACE");
    }

    build_request(argv);
    hash_slots(key);

    return check_status();
}

int redis_key::scan(int cursor, std::vector<std::string>& result,
                    const char * pattern /*= NULL*/, int count /*= 10*/)
{
    scan_keys("SCAN", NULL, cursor, pattern, count);
    return get_cursor_array(&result);
}

int redis_key::sort(const std::string& key, const std::string& dest,
                    sort_params* params /*= NULL*/)
{
    std::vector<std::string> argv;
    argv.push_back("SORT");
    argv.push_back(key);
    if (params) {
        argv.insert(argv.end(), params->get_params().begin(),
                    params->get_params().end());
    }
    argv.push_back("STORE");
    argv.push_back(dest);

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

int redis_key::sort(const std::string& key, std::vector<std::string>& result,
                    sort_params* params /*= NULL*/)
{
    std::vector<std::string> argv;
    argv.push_back("SORT");
    argv.push_back(key);
    if (params) {
        argv.insert(argv.end(), params->get_params().begin(),
                    params->get_params().end());
    }

    build_request(argv);
    hash_slots(key);

    return get_array_or_nil(result);
}

int redis_key::touch(const std::vector<std::string>& keys)
{
    std::vector<std::string> argv;
    argv.push_back("TOUCH");

    for (size_t i = 0; i < keys.size(); i++) {
        argv.push_back(keys[i]);
    }

    build_request(argv);

    if (!keys.empty()) {
        hash_slots(keys[0]);
    }

    return get_integer32();
}

int redis_key::touch(const std::string& key)
{
    std::vector<std::string> keys;
    keys.push_back(key);

    return touch(keys);
}

long long redis_key::ttl(const std::string & key)
{
    std::vector<std::string> argv;
    argv.push_back("TTL");
    argv.push_back(key);

    build_request(argv);
    hash_slots(key);

    return get_integer64();
}

std::string redis_key::type(const std::string & key)
{
    std::vector<std::string> argv;
    argv.push_back("TYPE");
    argv.push_back(key);

    build_request(argv);
    hash_slots(key);

    std::string result;
    get_string(result);
    return result;
}

/**
 * sort_params¿‡∂®“Â
 */
sort_params& sort_params::by(const std::string& pattern)
{
    params.push_back("BY");
    params.push_back(pattern);
    return *this;
}

sort_params& sort_params::nosort()
{
    params.push_back("BY");
    params.push_back("NOSORT");
    return *this;
}

sort_params& sort_params::get(const std::string& pattern)
{
    params.push_back("GET");
    params.push_back(pattern);
    return *this;
}

sort_params& sort_params::limit(int offset, int count)
{
    params.push_back("LIMIT");
    params.push_back(TO_STRING(offset));
    params.push_back(TO_STRING(count));
    return *this;
}

sort_params& sort_params::asc()
{
    params.push_back("ASC");
    return *this;
}

sort_params& sort_params::desc()
{
    params.push_back("DESC");
    return *this;
}

sort_params& sort_params::alpha()
{
    params.push_back("ALPHA");
    return *this;
}

