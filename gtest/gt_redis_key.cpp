#include <unistd.h>
#include <time.h>
#include <string>

#include "gtest/gtest.h"
#include "gt_redis_key.h"
#include "gt_common.h"

#include "redis_client.h"
#include "redis_string.h"
#include "redis_key.h"
#include "redis_helper.h"

redis_client* redis_key_test::m_pClient = NULL;
redis_string* redis_key_test::m_pStr = NULL;
redis_key* redis_key_test::m_pKey = NULL;

void redis_key_test::SetUpTestCase() {
    m_pClient = new redis_client(gt_component::Instance().get_host(),
                                 gt_component::Instance().get_port());
    m_pStr = new redis_string(m_pClient);
    m_pKey = new redis_key(m_pClient);

    m_pKey->del("foo");
}

void redis_key_test::TearDownTestCase() {
    delete m_pClient;
    m_pClient = NULL;
    delete m_pStr;
    m_pStr = NULL;
    delete m_pKey;
    m_pKey = NULL;
}

void redis_key_test::SetUp() {
    m_pStr->set("foo", "hello");
}

void redis_key_test::TearDown() {
    m_pKey->del("foo");
}

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

    // ɾ��һ��key
    EXPECT_EQ(1, redis_key_test::m_pKey->del("foo"));
    EXPECT_EQ(0, redis_key_test::m_pStr->get("foo", result));

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
    redis_key_test::m_pStr->mset(key_value); // ������������ֵ��

    // ɾ�����key
    EXPECT_EQ(5, redis_key_test::m_pKey->del(keys_del));
    EXPECT_EQ(true, redis_key_test::m_pStr->mget(keys, results));
    EXPECT_EQ(",2,4,6,8,10",
              redis_helper::join(results, ","));
    results.clear();

    // δɾ��key
    EXPECT_EQ(0, redis_key_test::m_pKey->del(keys_del));

    // ɾ������key
    std::vector<std::string> other_keys;
    other_keys.push_back("{foo}:1");
    other_keys.push_back("{foo}:2");
    other_keys.push_back("{foo}:3");
    other_keys.push_back("{foo}:4");
    EXPECT_EQ(2, redis_key_test::m_pKey->del(other_keys));
    EXPECT_EQ(true, redis_key_test::m_pStr->mget(keys, results));
    EXPECT_EQ(",,,,,6,8,10", redis_helper::join(results, ","));
    results.clear();

    redis_key_test::m_pKey->del(keys);
}

TEST_F(redis_key_test, dump)
{
    std::string result;

    EXPECT_EQ(1, redis_key_test::m_pKey->dump("foo", result));
    EXPECT_EQ("", result);

    redis_key_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_key_test::m_pKey->dump("foo", result));
}

TEST_F(redis_key_test, exists_multikey)
{
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");
    keys.push_back("foo");

    // ���key, �ظ�
    EXPECT_EQ(2, redis_key_test::m_pKey->exists(keys));

    // key������
    redis_key_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_key_test::m_pKey->exists(keys));
}

TEST_F(redis_key_test, exists)
{
    // key����
    EXPECT_EQ(1, redis_key_test::m_pKey->exists("foo"));

    // key������
    redis_key_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_key_test::m_pKey->exists("foo"));
}

TEST_F(redis_key_test, expire)
{
    // ��������ʱ��
    EXPECT_EQ(1, redis_key_test::m_pKey->expire("foo", 3));
    sleep(2);
    EXPECT_EQ(1, redis_key_test::m_pKey->exists("foo"));
    sleep(2);
    EXPECT_EQ(0, redis_key_test::m_pKey->exists("foo"));

    // ����ʱ�������
    redis_key_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(1, redis_key_test::m_pKey->exists("foo"));
    EXPECT_EQ(1, redis_key_test::m_pKey->expire("foo", -3));
    EXPECT_EQ(0, redis_key_test::m_pKey->exists("foo"));

    // key������
    redis_key_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_key_test::m_pKey->expire("foo", 3));
}

TEST_F(redis_key_test, expireat)
{
    // ����ʱ���
    EXPECT_EQ(1, redis_key_test::m_pKey->expireat("foo", time(0) + 3));
    sleep(2);
    EXPECT_EQ(1, redis_key_test::m_pKey->exists("foo"));
    sleep(2);
    EXPECT_EQ(0, redis_key_test::m_pKey->exists("foo"));

    // ʱ�����ʾ��ȥ��ʱ��
    redis_key_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(1, redis_key_test::m_pKey->exists("foo"));
    EXPECT_EQ(1, redis_key_test::m_pKey->expireat("foo", time(0) - 3));
    EXPECT_EQ(0, redis_key_test::m_pKey->exists("foo"));

    // key������
    redis_key_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_key_test::m_pKey->expireat("foo", time(0) + 3));
}

TEST_F(redis_key_test, keys)
{
    std::vector<std::string> result;
    redis_key_test::m_pKey->del("foo");
    redis_key_test::m_pStr->set("{K}:foo", "hello");
    redis_key_test::m_pStr->set("{K}:fao", "hello");
    redis_key_test::m_pStr->set("{K}:feeeo", "hello");
    redis_key_test::m_pStr->set("{K}:fco", "hello");
    redis_key_test::m_pStr->set("{K}:fo", "hello");

    EXPECT_EQ(5, redis_key_test::m_pKey->keys("*", result));
    EXPECT_EQ(true, contain_with("{K}:foo,{K}:fao,{K}:feeeo,{K}:fco,{K}:fo",result));
    result.clear();

    EXPECT_EQ(3, redis_key_test::m_pKey->keys("{K}:f?o", result));
    EXPECT_EQ(true, contain_with("{K}:foo,{K}:fao,{K}:fco",result));
    result.clear();

    EXPECT_EQ(5, redis_key_test::m_pKey->keys("{K}:f*o", result));
    EXPECT_EQ(true, contain_with("{K}:foo,{K}:fao,{K}:feeeo,{K}:fco,{K}:fo",result));
    result.clear();

    EXPECT_EQ(2, redis_key_test::m_pKey->keys("{K}:f[^o]o", result));
    EXPECT_EQ(true, contain_with("{K}:fao,{K}:fco",result));
    result.clear();

    EXPECT_EQ(2, redis_key_test::m_pKey->keys("{K}:f[a-c]o", result));
    EXPECT_EQ(true, contain_with("{K}:fao,{K}:fco",result));
    result.clear();

    redis_key_test::m_pKey->del("{K}:foo");
    redis_key_test::m_pKey->del("{K}:fao");
    redis_key_test::m_pKey->del("{K}:feeeo");
    redis_key_test::m_pKey->del("{K}:fco");
    redis_key_test::m_pKey->del("{K}:fo");
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
    redis_key_test::m_pStr->mset(key_value);

    int cursor = redis_key_test::m_pKey->scan(0, result, "*{foo}*");
    while (cursor != 0) {
        cursor = redis_key_test::m_pKey->scan(cursor, result, "*{foo}*");
    }
    EXPECT_EQ(20, result.size());

    redis_key_test::m_pKey->del(result);
}

