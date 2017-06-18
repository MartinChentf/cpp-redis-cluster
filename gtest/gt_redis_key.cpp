#include <unistd.h>
#include <time.h>
#include <string>

#include "gtest/gtest.h"
#include "gt_redis_key.h"
#include "gt_common.h"

#include "Util.h"
#include "redis_log.h"
#include "redis.h"

redis* redis_key_test::m_pRedis = NULL;

void redis_key_test::SetUpTestCase() {
    m_pRedis = new redis(gt_component::Instance().get_host(0),
                         gt_component::Instance().get_port(0));
}

void redis_key_test::TearDownTestCase() {
    delete m_pRedis;
    m_pRedis = NULL;
}

void redis_key_test::SetUp() {}

void redis_key_test::TearDown() {}

bool redis_key_test::contain_with(std::string str,
                                  std::vector<std::string>& search)
{
    bool iret = true;
    for (size_t i = 0; i < search.size(); i++) {
        if (!strstr(str.c_str(), search[i].c_str())) {
            iret = false;
            break;
        }
    }
    return iret;
}

TEST_F(redis_key_test, del)
{
    std::string result;

    redis_key_test::m_pRedis->set("foo", "hello");

    // ɾ��һ��key
    EXPECT_EQ(1, redis_key_test::m_pRedis->del("foo"));
    EXPECT_EQ(0, redis_key_test::m_pRedis->get("foo", result));

    std::vector<std::string*> results;
    std::vector<std::string> keys;
    std::vector<std::string> keys_del;
    std::map<std::string, std::string> key_value;
    for (int i = 1; i <= 10; i++) {
        std::string key("{foo}:");
        key += TO_STRING(i);
        key_value[key] = TO_STRING(i);
        keys.push_back(key);
        if (i%2) keys_del.push_back(key);
    }
    redis_key_test::m_pRedis->mset(key_value); // ������������ֵ��

    // ɾ�����key
    EXPECT_EQ(5, redis_key_test::m_pRedis->del(keys_del));
    EXPECT_EQ(true, redis_key_test::m_pRedis->mget(keys, results));
    EXPECT_EQ(",2,4,6,8,10",
              Util::join(results, ","));
    results.clear();

    // δɾ��key
    EXPECT_EQ(0, redis_key_test::m_pRedis->del(keys_del));

    // ɾ������key
    std::vector<std::string> other_keys;
    other_keys.push_back("{foo}:1");
    other_keys.push_back("{foo}:2");
    other_keys.push_back("{foo}:3");
    other_keys.push_back("{foo}:4");
    EXPECT_EQ(2, redis_key_test::m_pRedis->del(other_keys));
    EXPECT_EQ(true, redis_key_test::m_pRedis->mget(keys, results));
    EXPECT_EQ(",,,,,6,8,10", Util::join(results, ","));
    results.clear();

    redis_key_test::m_pRedis->del(keys);
}

TEST_F(redis_key_test, dump)
{
    std::string result;

    redis_key_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(1, redis_key_test::m_pRedis->dump("foo", result));
    EXPECT_EQ(0, memcmp("\0\x5hello\a\0\x9C@\n\x85m\xFE\xF5\x10",
                        result.c_str(), 17));

    redis_key_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_key_test::m_pRedis->dump("foo", result));
}

TEST_F(redis_key_test, exists_multikey)
{
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");
    keys.push_back("foo");

    // ���key, �ظ�
    redis_key_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(2, redis_key_test::m_pRedis->exists(keys));

    // key������
    redis_key_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_key_test::m_pRedis->exists(keys));
}

TEST_F(redis_key_test, exists)
{
    redis_key_test::m_pRedis->set("foo", "hello");
    // key����
    EXPECT_EQ(1, redis_key_test::m_pRedis->exists("foo"));

    // key������
    redis_key_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_key_test::m_pRedis->exists("foo"));
}

TEST_F(redis_key_test, expire)
{
    // ��������ʱ��
    redis_key_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(1, redis_key_test::m_pRedis->expire("foo", 3));
    sleep(2);
    EXPECT_EQ(1, redis_key_test::m_pRedis->exists("foo"));
    sleep(2);
    EXPECT_EQ(0, redis_key_test::m_pRedis->exists("foo"));

    // ����ʱ�������
    redis_key_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(1, redis_key_test::m_pRedis->exists("foo"));
    EXPECT_EQ(1, redis_key_test::m_pRedis->expire("foo", -3));
    EXPECT_EQ(0, redis_key_test::m_pRedis->exists("foo"));

    // key������
    redis_key_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_key_test::m_pRedis->expire("foo", 3));
}

TEST_F(redis_key_test, expireat)
{
    // ����ʱ���
    redis_key_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(1, redis_key_test::m_pRedis->expireat("foo", time(0) + 3));
    sleep(2);
    EXPECT_EQ(1, redis_key_test::m_pRedis->exists("foo"));
    sleep(2);
    EXPECT_EQ(0, redis_key_test::m_pRedis->exists("foo"));

    // ʱ�����ʾ��ȥ��ʱ��
    redis_key_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(1, redis_key_test::m_pRedis->exists("foo"));
    EXPECT_EQ(1, redis_key_test::m_pRedis->expireat("foo", time(0) - 3));
    EXPECT_EQ(0, redis_key_test::m_pRedis->exists("foo"));

    // key������
    redis_key_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_key_test::m_pRedis->expireat("foo", time(0) + 3));
}

TEST_F(redis_key_test, keys)
{
    std::vector<std::string> result;
    redis_key_test::m_pRedis->set("{K}:foo", "hello");
    redis_key_test::m_pRedis->set("{K}:fao", "hello");
    redis_key_test::m_pRedis->set("{K}:feeeo", "hello");
    redis_key_test::m_pRedis->set("{K}:fco", "hello");
    redis_key_test::m_pRedis->set("{K}:fo", "hello");

    EXPECT_EQ(5, redis_key_test::m_pRedis->keys("{K}:*", result));
    EXPECT_EQ(true, contain_with("{K}:foo,{K}:fao,{K}:feeeo,{K}:fco,{K}:fo",result));
    result.clear();

    EXPECT_EQ(3, redis_key_test::m_pRedis->keys("{K}:f?o", result));
    EXPECT_EQ(true, contain_with("{K}:foo,{K}:fao,{K}:fco",result));
    result.clear();

    EXPECT_EQ(5, redis_key_test::m_pRedis->keys("{K}:f*o", result));
    EXPECT_EQ(true, contain_with("{K}:foo,{K}:fao,{K}:feeeo,{K}:fco,{K}:fo",result));
    result.clear();

    EXPECT_EQ(2, redis_key_test::m_pRedis->keys("{K}:f[^o]o", result));
    EXPECT_EQ(true, contain_with("{K}:fao,{K}:fco",result));
    result.clear();

    EXPECT_EQ(2, redis_key_test::m_pRedis->keys("{K}:f[a-c]o", result));
    EXPECT_EQ(true, contain_with("{K}:fao,{K}:fco",result));
    result.clear();

    redis_key_test::m_pRedis->del("{K}:foo");
    redis_key_test::m_pRedis->del("{K}:fao");
    redis_key_test::m_pRedis->del("{K}:feeeo");
    redis_key_test::m_pRedis->del("{K}:fco");
    redis_key_test::m_pRedis->del("{K}:fo");
}

TEST_F(redis_key_test, migrate)
{
    // ������һ�� server
    redis* pRedis = new redis(gt_component::Instance().get_host(1),
                             gt_component::Instance().get_port(1));
    //pRedis->auth(gt_component::Instance().get_passwd(1));

    std::string result;
    // ��ָ��keyǨ�Ƶ�Ŀ��ʵ��
    redis_key_test::m_pRedis->set("foo", "hello");
    pRedis->del("foo");
    redis_key_test::m_pRedis->migrate(gt_component::Instance().get_host(1),
        gt_component::Instance().get_port(1), "foo", 0, 1000);

    EXPECT_EQ(0, redis_key_test::m_pRedis->exists("foo"));
    EXPECT_EQ(1, pRedis->get("foo", result));
    EXPECT_EQ("hello", result);

    redis_key_test::m_pRedis->del("foo");
    pRedis->del("foo");
    SAFE_DELETE(pRedis);
}

TEST_F(redis_key_test, migrate_copy)
{
    // ������һ�� server
    redis* pRedis = new redis(gt_component::Instance().get_host(1),
                             gt_component::Instance().get_port(1));
    //pRedis->auth(gt_component::Instance().get_passwd(1));

    std::string result;
    // ʹ�� COPY ѡ��
    redis_key_test::m_pRedis->set("foo", "hello");
    pRedis->del("foo");
    MigrateParams* copyParam = new MigrateParams();
    copyParam->copy();
    redis_key_test::m_pRedis->migrate(gt_component::Instance().get_host(1),
        gt_component::Instance().get_port(1), "foo", 0, 1000, copyParam);

    EXPECT_EQ(1, redis_key_test::m_pRedis->get("foo", result));
    EXPECT_EQ("hello", result);
    EXPECT_EQ(1, pRedis->get("foo", result));
    EXPECT_EQ("hello", result);

    redis_key_test::m_pRedis->del("foo");
    pRedis->del("foo");
    SAFE_DELETE(copyParam);
    SAFE_DELETE(pRedis);
}

TEST_F(redis_key_test, migrate_replace)
{
    // ������һ�� server
    redis* pRedis = new redis(gt_component::Instance().get_host(1),
                             gt_component::Instance().get_port(1));
    //pRedis->auth(gt_component::Instance().get_passwd(1));

    std::string result;
    // ʹ�� REPLACE ѡ��
    redis_key_test::m_pRedis->set("foo", "hello");
    pRedis->set("foo", "world");

    MigrateParams* replaceParam = new MigrateParams();
    replaceParam->replace();
    redis_key_test::m_pRedis->migrate(gt_component::Instance().get_host(1),
        gt_component::Instance().get_port(1), "foo", 0, 1000, replaceParam);

    EXPECT_EQ(0, redis_key_test::m_pRedis->get("foo", result));
    EXPECT_EQ(1, pRedis->get("foo", result));
    EXPECT_EQ("hello", result);

    redis_key_test::m_pRedis->del("foo");
    pRedis->del("foo");
    SAFE_DELETE(replaceParam);
    SAFE_DELETE(pRedis);
}

TEST_F(redis_key_test, migrate_keys)
{
    // ������һ�� server
    redis* pRedis = new redis(gt_component::Instance().get_host(1),
                             gt_component::Instance().get_port(1));
    //pRedis->auth(gt_component::Instance().get_passwd(1));

    std::string result;
    // ʹ�� KEYS ѡ��
    redis_key_test::m_pRedis->set("foo1", "hello");
    redis_key_test::m_pRedis->set("foo2", "world");

    std::vector<std::string> vkeys;
    vkeys.push_back("foo1");
    vkeys.push_back("foo2");
    MigrateParams* keysParam = new MigrateParams();
    keysParam->keys(vkeys);

    redis_key_test::m_pRedis->migrate(gt_component::Instance().get_host(1),
        gt_component::Instance().get_port(1), "", 0, 1000, keysParam);

    std::vector<std::string*> values;
    EXPECT_EQ(true, redis_key_test::m_pRedis->mget(vkeys, values));
    values.clear();
    EXPECT_EQ(true, pRedis->mget(vkeys, values));
    EXPECT_EQ("hello", *values[0]);
    EXPECT_EQ("world", *values[1]);

    for (size_t i = 0; i < values.size(); i++) {
        delete values[i];
    }
    redis_key_test::m_pRedis->del(vkeys);
    pRedis->del(vkeys);
    SAFE_DELETE(keysParam);
    SAFE_DELETE(pRedis);
}

TEST_F(redis_key_test, move)
{
    // ���������
    redis_key_test::m_pRedis->select(1);
    redis_key_test::m_pRedis->del("foo");

    /**
     * ��ָ��key��db0�Ƶ�db1
     * ����: ָ��key��db1��, ����db0��
     */
    redis_key_test::m_pRedis->select(0);
    redis_key_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(1, redis_key_test::m_pRedis->move("foo", 1));

    std::string result;
    EXPECT_EQ(0, redis_key_test::m_pRedis->exists("foo"));
    redis_key_test::m_pRedis->select(1);
    EXPECT_EQ(1, redis_key_test::m_pRedis->get("foo", result));
    EXPECT_EQ("hello", result);

    /**
     * ָ��key���ڵ�ǰ���ݿ�(db0)��, �ƶ�ʧ��
     */
    redis_key_test::m_pRedis->select(0);
    EXPECT_EQ(0, redis_key_test::m_pRedis->move("foo", 1));

    /**
     * Ŀ�����ݿ�(db1)����ͬ��key, �ƶ�ʧ��
     */
    redis_key_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(0, redis_key_test::m_pRedis->move("foo", 1));

    redis_key_test::m_pRedis->del("foo");
    redis_key_test::m_pRedis->select(1);
    redis_key_test::m_pRedis->del("foo");
    redis_key_test::m_pRedis->select(0);
}

TEST_F(redis_key_test, object_refcount)
{
    redis_key_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(1, redis_key_test::m_pRedis->object_refcount("foo"));

    // key�����ڵ����
    redis_key_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_key_test::m_pRedis->object_refcount("foo"));
}

TEST_F(redis_key_test, object_encoding)
{
    std::string encode;
    redis_key_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(1, redis_key_test::m_pRedis->object_encoding("foo", encode));
    EXPECT_EQ("embstr", encode);

    // key�����ڵ����
    encode = "";
    redis_key_test::m_pRedis->del("foo");
    EXPECT_EQ(1, redis_key_test::m_pRedis->object_encoding("foo", encode));
    EXPECT_EQ("", encode);
}

TEST_F(redis_key_test, object_idletime)
{
    redis_key_test::m_pRedis->set("foo", "hello");

    EXPECT_EQ(0, redis_key_test::m_pRedis->object_idletime("foo"));

    redis_key_test::m_pRedis->del("foo");    
}

TEST_F(redis_key_test, persist)
{
    /**
     * ȡ��ָ��key������ʱ��, ����ʱ�䵽�ں�key������
     */
    redis_key_test::m_pRedis->set("foo", "hello");
    redis_key_test::m_pRedis->expire("foo", 3); // ��������ʱ��
    sleep(2);
    EXPECT_EQ(1, redis_key_test::m_pRedis->persist("foo"));
    sleep(2);
    EXPECT_EQ(1, redis_key_test::m_pRedis->exists("foo"));

    /**
     * keyû����������ʱ������
     */
    EXPECT_EQ(0, redis_key_test::m_pRedis->persist("foo"));

    /**
     * key�����ڵ����
     */
    redis_key_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_key_test::m_pRedis->persist("foo"));
}

TEST_F(redis_key_test, pexpire)
{
    // ��������ʱ��
    redis_key_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(1, redis_key_test::m_pRedis->pexpire("foo", 3000));
    sleep(2);
    EXPECT_EQ(1, redis_key_test::m_pRedis->exists("foo"));
    sleep(2);
    EXPECT_EQ(0, redis_key_test::m_pRedis->exists("foo"));

    // ����ʱ�������
    redis_key_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(1, redis_key_test::m_pRedis->exists("foo"));
    EXPECT_EQ(1, redis_key_test::m_pRedis->pexpire("foo", -3000));
    EXPECT_EQ(0, redis_key_test::m_pRedis->exists("foo"));

    // key������
    redis_key_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_key_test::m_pRedis->pexpire("foo", 3));
}

TEST_F(redis_key_test, pexpireat)
{
    // ����ʱ���
    redis_key_test::m_pRedis->set("foo", "hello");
    unsigned long long milliTimestamp = time(0) * 1000 + 3000;
    EXPECT_EQ(1, redis_key_test::m_pRedis->pexpireat("foo", milliTimestamp));
    sleep(2);
    EXPECT_EQ(1, redis_key_test::m_pRedis->exists("foo"));
    sleep(2);
    EXPECT_EQ(0, redis_key_test::m_pRedis->exists("foo"));

    // ʱ�����ʾ��ȥ��ʱ��
    redis_key_test::m_pRedis->set("foo", "hello");
    milliTimestamp = time(0) * 1000 - 3000;
    EXPECT_EQ(1, redis_key_test::m_pRedis->exists("foo"));
    EXPECT_EQ(1, redis_key_test::m_pRedis->pexpireat("foo", milliTimestamp));
    EXPECT_EQ(0, redis_key_test::m_pRedis->exists("foo"));

    // key������
    redis_key_test::m_pRedis->del("foo");
    milliTimestamp = time(0) * 1000 + 3000;
    EXPECT_EQ(0, redis_key_test::m_pRedis->pexpireat("foo", milliTimestamp));
}

TEST_F(redis_key_test, pttl)
{
    /**
     * key ������
     */
    long pttl = redis_key_test::m_pRedis->pttl("foo");
    EXPECT_EQ(-2, pttl);

    /**
     * key ���ڵ���û�й�������ʱ��
     */
    redis_key_test::m_pRedis->set("foo", "hello");
    pttl = redis_key_test::m_pRedis->pttl("foo");
    EXPECT_EQ(-1, pttl);

    /**
     * key ���ڵ��ǹ���ʱ���ѱ�ȡ��
     */
    redis_key_test::m_pRedis->pexpire("foo", 4000);
    redis_key_test::m_pRedis->persist("foo");
    pttl = redis_key_test::m_pRedis->pttl("foo");
    EXPECT_EQ(-1, pttl);

    /**
     * ���� key ʣ�������ʱ��
     */
    redis_key_test::m_pRedis->pexpire("foo", 4000);
    pttl = redis_key_test::m_pRedis->pttl("foo");
    EXPECT_TRUE(pttl >= 0 && pttl <= 4000);

    redis_key_test::m_pRedis->del("foo");
}

TEST_F(redis_key_test, randomkey)
{
    /**
     * ��ǰ���ݿ�Ϊ��
     */
    std::string key;
    redis_key_test::m_pRedis->flushAll(); // ������ݿ�
    EXPECT_EQ(0, redis_key_test::m_pRedis->randomkey(key));

    /**
     * ����������ݿ��е�key
     */
    redis_key_test::m_pRedis->set("foo", "bar");
    EXPECT_EQ(1, redis_key_test::m_pRedis->randomkey(key));
    EXPECT_EQ("foo", key);

    redis_key_test::m_pRedis->set("bar", "foo");
    EXPECT_EQ(1, redis_key_test::m_pRedis->randomkey(key));
    EXPECT_TRUE(key.compare("foo") == 0 || key.compare("bar") == 0);

    redis_key_test::m_pRedis->del("foo");
    redis_key_test::m_pRedis->del("bar");
}

TEST_F(redis_key_test, rename)
{
    /**
     * key ������
     */
    EXPECT_EQ(false, redis_key_test::m_pRedis->rename("foo", "bar"));

    /**
     * ���������ݿ��е�key
     */
    redis_key_test::m_pRedis->set("foo", "bar");
    EXPECT_EQ(true, redis_key_test::m_pRedis->rename("foo", "bar"));

    EXPECT_EQ(0, redis_key_test::m_pRedis->exists("foo"));
    std::string result;
    redis_key_test::m_pRedis->get("bar", result);
    EXPECT_EQ("bar", result);

    /**
     * new_key ����
     */
    redis_key_test::m_pRedis->set("foo", "foo");
    EXPECT_EQ(true, redis_key_test::m_pRedis->rename("foo", "bar"));
    redis_key_test::m_pRedis->get("bar", result);
    EXPECT_EQ("foo", result);
    
    redis_key_test::m_pRedis->del("bar");
}

TEST_F(redis_key_test, renamenx)
{
    /**
     * key ������
     */
    EXPECT_EQ(-1, redis_key_test::m_pRedis->renamenx("foo", "bar"));

    /**
     * ���������ݿ��е�key
     */
    redis_key_test::m_pRedis->set("foo", "bar");
    EXPECT_EQ(1, redis_key_test::m_pRedis->renamenx("foo", "bar"));

    EXPECT_EQ(0, redis_key_test::m_pRedis->exists("foo"));
    std::string result;
    redis_key_test::m_pRedis->get("bar", result);
    EXPECT_EQ("bar", result);

    /**
     * new_key ����
     */
    redis_key_test::m_pRedis->set("foo", "foo");
    EXPECT_EQ(0, redis_key_test::m_pRedis->renamenx("foo", "bar"));
    redis_key_test::m_pRedis->get("foo", result);
    EXPECT_EQ("foo", result);
    redis_key_test::m_pRedis->get("bar", result);
    EXPECT_EQ("bar", result);

    redis_key_test::m_pRedis->del("foo");
    redis_key_test::m_pRedis->del("bar");
}

TEST_F(redis_key_test, restore)
{
    std::string serialValue;
    redis_key_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(1, redis_key_test::m_pRedis->dump("foo", serialValue));
    EXPECT_EQ(0, memcmp("\0\x5hello\a\0\x9C@\n\x85m\xFE\xF5\x10",
                        serialValue.c_str(), 17));

    /**
     * key �Ѵ���
     */
    EXPECT_EQ(false, redis_key_test::m_pRedis->restore("foo", 0, serialValue));

    /**
     * key ������
     */
    EXPECT_EQ(true, redis_key_test::m_pRedis->restore("bar", 0, serialValue));
    std::string result;
    redis_key_test::m_pRedis->get("bar", result);
    EXPECT_EQ("hello", result);

    /**
     * ʹ�� REPLACE ģʽ
     */
    redis_key_test::m_pRedis->set("bar", "world");
    EXPECT_EQ(true, redis_key_test::m_pRedis->restore("bar", 0, serialValue, true));
    redis_key_test::m_pRedis->get("bar", result);
    EXPECT_EQ("hello", result);

    redis_key_test::m_pRedis->del("foo");
    redis_key_test::m_pRedis->del("bar");
}

TEST_F(redis_key_test, scan)
{
    std::vector<std::string> result;
    std::map<std::string, std::string> key_value;
    for (int i = 1; i <= 20; i++) {
        std::string key("{foo}:");
        key += TO_STRING(i);
        key_value[key] = TO_STRING(i);
    }
    redis_key_test::m_pRedis->mset(key_value);

    int cursor = redis_key_test::m_pRedis->scan(0, result, "*{foo}*");
    while (cursor > 0) {
        cursor = redis_key_test::m_pRedis->scan(cursor, result, "*{foo}*");
    }
    EXPECT_EQ(20, result.size());

    redis_key_test::m_pRedis->del(result);
}

TEST_F(redis_key_test, sort)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("23");
    values.push_back("15");
    values.push_back("110");
    values.push_back("7");
    redis_key_test::m_pRedis->rpush("sort-input", values);

    /**
     * ������ֵ˳������
     */
    redis_key_test::m_pRedis->sort("sort-input", result);
    EXPECT_EQ("7,15,23,110", Util::join(result, ","));
    result.clear();

    /**
     * ������ĸ˳������
     */
    SortParams alpha;
    redis_key_test::m_pRedis->sort("sort-input", result, &alpha.alpha());
    EXPECT_EQ("110,15,23,7", Util::join(result, ","));
    result.clear();

    /**
     * ͨ���ⲿ key ����
     */
    redis_key_test::m_pRedis->hset("h-7", "field", "5");
    redis_key_test::m_pRedis->hset("h-15", "field", "1");
    redis_key_test::m_pRedis->hset("h-23", "field", "9");
    redis_key_test::m_pRedis->hset("h-110", "field", "3");

    SortParams param;
    redis_key_test::m_pRedis->sort("sort-input", result, &param.by("h-*->field"));
    EXPECT_EQ("15,110,7,23", Util::join(result, ","));
    result.clear();
    
    redis_key_test::m_pRedis->sort("sort-input", result, &param.get("h-*->field"));
    EXPECT_EQ("1,3,5,9", Util::join(result, ","));
    result.clear();

    redis_key_test::m_pRedis->del("sort-input");
    redis_key_test::m_pRedis->del("h-7");
    redis_key_test::m_pRedis->del("h-15");
    redis_key_test::m_pRedis->del("h-23");
    redis_key_test::m_pRedis->del("h-110");
}

TEST_F(redis_key_test, touch)
{
    redis_key_test::m_pRedis->set("foo", "bar");
    sleep(5);
    long long idletime = redis_key_test::m_pRedis->object_idletime("foo");
    EXPECT_TRUE(idletime >= 5);

    EXPECT_EQ(1, redis_key_test::m_pRedis->touch("foo"));
    idletime = redis_key_test::m_pRedis->object_idletime("foo");
    EXPECT_TRUE(idletime < 5);

    redis_key_test::m_pRedis->del("foo");
}

TEST_F(redis_key_test, ttl)
{
    /**
     * key ������
     */
    long ttl = redis_key_test::m_pRedis->ttl("foo");
    EXPECT_EQ(-2, ttl);

    /**
     * key ���ڵ���û�й�������ʱ��
     */
    redis_key_test::m_pRedis->set("foo", "hello");
    ttl = redis_key_test::m_pRedis->ttl("foo");
    EXPECT_EQ(-1, ttl);

    /**
     * key ���ڵ��ǹ���ʱ���ѱ�ȡ��
     */
    redis_key_test::m_pRedis->pexpire("foo", 4);
    redis_key_test::m_pRedis->persist("foo");
    ttl = redis_key_test::m_pRedis->ttl("foo");
    EXPECT_EQ(-1, ttl);

    /**
     * ���� key ʣ�������ʱ��
     */
    redis_key_test::m_pRedis->pexpire("foo", 4);
    ttl = redis_key_test::m_pRedis->ttl("foo");
    EXPECT_TRUE(ttl >= 0 && ttl <= 4);

    redis_key_test::m_pRedis->del("foo");
}

TEST_F(redis_key_test, type)
{
    /**
     * key ������
     */
    std::string type = redis_key_test::m_pRedis->type("foo");
    EXPECT_EQ("none", type);

    /**
     * key Ϊstring����
     */
    redis_key_test::m_pRedis->set("foo", "bar");
    type = redis_key_test::m_pRedis->type("foo");
    EXPECT_EQ("string", type);

    /**
     * key Ϊlist����
     */
    redis_key_test::m_pRedis->del("foo");
    redis_key_test::m_pRedis->hset("foo", "bar", "hello");
    type = redis_key_test::m_pRedis->type("foo");
    EXPECT_EQ("hash", type);

    redis_key_test::m_pRedis->del("foo");
}

