#include <pthread.h>

#include "gtest/gtest.h"
#include "gt_redis_list.h"
#include "gt_common.h"

#include "redis_client.h"
#include "redis_list.h"
#include "redis_key.h"
#include "redis_helper.h"
#include "redis_string.h"

redis_client* redis_list_test::m_pClient = NULL;
redis_string* redis_list_test::m_pStr = NULL;
redis_list* redis_list_test::m_pList = NULL;
redis_key* redis_list_test::m_pKey = NULL;

void redis_list_test::SetUpTestCase() {
    m_pClient = new redis_client(gt_component::Instance().get_cluster_host(),
                                 gt_component::Instance().get_cluster_port());
    m_pList = new redis_list(m_pClient);
    m_pKey = new redis_key(m_pClient);
    m_pStr = new redis_string(m_pClient);

    m_pKey->del("foo");
}

void redis_list_test::TearDownTestCase() {
    delete m_pClient;
    m_pClient = NULL;
    delete m_pList;
    m_pList = NULL;
    delete m_pKey;
    m_pKey = NULL;
    delete m_pStr;
    m_pStr = NULL;
}

void redis_list_test::SetUp()
{
    std::vector<std::string> values;
    values.push_back("one");
    values.push_back("two");
    values.push_back("three");

    redis_list_test::m_pList->lpush("foo", values);
}

void redis_list_test::TearDown()
{
    redis_list_test::m_pKey->del("foo");
}

TEST_F(redis_list_test, lrange)
{
    std::vector<std::string> result;

    // ����ȫ��Ԫ��
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, two, one", redis_helper::join(result, ", "));
    result.clear();

    // ���ز���Ԫ��
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 1, 2, result));
    EXPECT_EQ("two, one", redis_helper::join(result, ", "));
    result.clear();

    // start > stop
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 2, 0, result));
    EXPECT_EQ("", redis_helper::join(result, ", "));
    result.clear();

    // stop����ʵ���б�ĩβԪ���±�
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 1, 10, result));
    EXPECT_EQ("two, one", redis_helper::join(result, ", "));
    result.clear();

    // key���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(false, redis_list_test::m_pList->lrange("foo", 1, 2, result));
}

TEST_F(redis_list_test, lpush)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("four");
    values.push_back("five");

    // ���б�ͷ��������Ԫ��
    EXPECT_EQ(5, redis_list_test::m_pList->lpush("foo", values));
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, result));
    EXPECT_EQ("five, four, three, two, one", redis_helper::join(result, ", "));
    
    // key���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pList->lpush("foo", values));
}

TEST_F(redis_list_test, lpushx)
{
    std::vector<std::string> result;

    // key���ڵ����
    EXPECT_EQ(4, redis_list_test::m_pList->lpushx("foo", "four"));
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, result));
    EXPECT_EQ("four, three, two, one", redis_helper::join(result, ", "));

    // key�����ڵ����
    redis_list_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pList->lpushx("foo", "four"));

    // key���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pList->lpushx("foo", "four"));
}

TEST_F(redis_list_test, lindex)
{
    std::string result;

    // 1. ��ȡindex����Ԫ��
    EXPECT_EQ(1, redis_list_test::m_pList->lindex("foo", 1, result));
    EXPECT_EQ("two", result);

    // 2. indexΪ��ֵ�����
    EXPECT_EQ(1, redis_list_test::m_pList->lindex("foo", -1, result));
    EXPECT_EQ("one", result);

    // 3. index������Χ�����
    EXPECT_EQ(0, redis_list_test::m_pList->lindex("foo", 3, result));

    // 4. key�����ڵ����(�൱��index������Χ)
    redis_list_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pList->lindex("foo", 0, result));
    
    // 5. key���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pList->lindex("foo", 0, result));
}

TEST_F(redis_list_test, linsert)
{
    std::vector<std::string> result;

    // ����ָ��Ԫ��֮ǰ
    EXPECT_EQ(4, redis_list_test::m_pList->linsert("foo", true, "two", "four"));
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, four, two, one", redis_helper::join(result, ", "));
    result.clear();

    // ����ָ��Ԫ��֮��
    EXPECT_EQ(5, redis_list_test::m_pList->linsert("foo", false, "two", "five"));
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, four, two, five, one", redis_helper::join(result, ", "));
    result.clear();

    // ָ��Ԫ�ز����ڵ����
    EXPECT_EQ(-1, redis_list_test::m_pList->linsert("foo", true, "six", "five"));

    // key�����ڵ����
    redis_list_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pList->linsert("foo", true, "two", "five"));

    // key���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pList->linsert("foo", true, "two", "four"));
}

TEST_F(redis_list_test, llen)
{
    // �����б���
    EXPECT_EQ(3, redis_list_test::m_pList->llen("foo"));

    // key�����ڵ����
    redis_list_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pList->llen("foo"));
    
    // key���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pList->llen("foo"));
}

TEST_F(redis_list_test, lpop)
{
    std::string result;

    // �����б��е�һ��Ԫ��
    EXPECT_EQ(1, redis_list_test::m_pList->lpop("foo", result));
    EXPECT_EQ("three", result);

    // key�����ڵ����
    redis_list_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pList->lpop("foo", result));
    
    // key���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pList->lpop("foo", result));
}

TEST_F(redis_list_test, lrem_from_head)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("two");
    values.push_back("one");
    values.push_back("two");
    redis_list_test::m_pList->lpush("foo", values);

    // ��ͷ����ʼ�Ƴ�
    EXPECT_EQ(2, redis_list_test::m_pList->lrem("foo", 2, "two"));
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, result));
    EXPECT_EQ("one, three, two, one", redis_helper::join(result, ", "));

    // vaule�����ڵ����
    EXPECT_EQ(0, redis_list_test::m_pList->lrem("foo", 2, "four"));

    // key�����ڵ����
    redis_list_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pList->lrem("foo", 2, "two"));

    // key���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pList->lrem("foo", 2, "two"));
}

TEST_F(redis_list_test, lrem_from_tail)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("two");
    values.push_back("one");
    values.push_back("two");
    redis_list_test::m_pList->lpush("foo", values);

    // ��β����ʼ�Ƴ�
    EXPECT_EQ(2, redis_list_test::m_pList->lrem("foo", -2, "two"));
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, result));
    EXPECT_EQ("two, one, three, one", redis_helper::join(result, ", "));
}

TEST_F(redis_list_test, lrem_all)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("two");
    values.push_back("one");
    values.push_back("two");
    redis_list_test::m_pList->lpush("foo", values);

    // �Ƴ�����
    EXPECT_EQ(3, redis_list_test::m_pList->lrem("foo", 0, "two"));
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, result));
    EXPECT_EQ("one, three, one", redis_helper::join(result, ", "));
}

TEST_F(redis_list_test, lset)
{
    std::vector<std::string> result;

    // ������ֵ
    EXPECT_EQ(true, redis_list_test::m_pList->lset("foo", 1, "four"));
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, four, one", redis_helper::join(result, ", "));

    // indexԽ������
    EXPECT_EQ(false, redis_list_test::m_pList->lset("foo", 3, "four"));
    
    // key�����ڵ����
    redis_list_test::m_pKey->del("foo");
    EXPECT_EQ(false, redis_list_test::m_pList->lset("foo", 1, "four"));

    // key���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(false, redis_list_test::m_pList->lset("foo", 1, "four"));
}

TEST_F(redis_list_test, ltrim)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("four");
    values.push_back("five");
    values.push_back("six");
    values.push_back("seven");
    redis_list_test::m_pList->lpush("foo", values);

    EXPECT_EQ(true, redis_list_test::m_pList->ltrim("foo", 2, 5));
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, result));
    EXPECT_EQ("five, four, three, two", redis_helper::join(result, ", "));

    // key�����ڵ����
    redis_list_test::m_pKey->del("foo");
    EXPECT_EQ(true, redis_list_test::m_pList->ltrim("foo", 2, 5));

    // key���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(false, redis_list_test::m_pList->ltrim("foo", 2, 5));
}

TEST_F(redis_list_test, ltrim_start_gt_stop)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("four");
    values.push_back("five");
    values.push_back("six");
    values.push_back("seven");
    redis_list_test::m_pList->lpush("foo", values);

    // start > stop
    EXPECT_EQ(true, redis_list_test::m_pList->ltrim("foo", 6, 3));
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, result));
    EXPECT_EQ("", redis_helper::join(result, ", "));
}

TEST_F(redis_list_test, ltrim_stop_gt_end)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("four");
    values.push_back("five");
    values.push_back("six");
    values.push_back("seven");
    redis_list_test::m_pList->lpush("foo", values);

    // stop�����б��±�
    EXPECT_EQ(true, redis_list_test::m_pList->ltrim("foo", 2, 10));
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, result));
    EXPECT_EQ("five, four, three, two, one", redis_helper::join(result, ", "));
}

TEST_F(redis_list_test, rpop)
{
    std::string result;

    // �����б��е�һ��Ԫ��
    EXPECT_EQ(1, redis_list_test::m_pList->rpop("foo", result));
    EXPECT_EQ("one", result);

    // key������
    redis_list_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pList->rpop("foo", result));
    
    // key���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pList->rpop("foo", result));
}

TEST_F(redis_list_test, rpoplpush)
{
    std::string result;
    std::vector<std::string> vaules;

    // ��srcĩβ����һ��Ԫ�ز�����destͷ��
    EXPECT_EQ(1, redis_list_test::m_pList->rpoplpush("foo", "{foo}1", result));
    EXPECT_EQ("one", result);
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("three, two", redis_helper::join(vaules, ", "));
    vaules.clear();
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("three, two", redis_helper::join(vaules, ", "));
    vaules.clear();

    // src�����ڵ����
    redis_list_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pList->rpoplpush("foo", "{foo}1", result));

    // src���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pList->rpoplpush("foo", "{foo}1", result));
}

TEST_F(redis_list_test, rpoplpush_src_same_with_dest)
{
    std::string result;
    std::vector<std::string> vaules;

    // ��srcĩβ����һ��Ԫ�ز�����destͷ��
    EXPECT_EQ(1, redis_list_test::m_pList->rpoplpush("foo", "foo", result));
    EXPECT_EQ("one", result);
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("one, three, two", redis_helper::join(vaules, ", "));

    // dest���ʹ�������
    redis_list_test::m_pStr->set("{foo}1", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pList->rpoplpush("foo", "{foo}1", result));

    redis_list_test::m_pKey->del("{foo}1");
}

TEST_F(redis_list_test, rpush)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("four");
    values.push_back("five");

    // ���б�ĩβ������Ԫ��
    EXPECT_EQ(5, redis_list_test::m_pList->rpush("foo", values));
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, two, one, four, five", redis_helper::join(result, ", "));

    // key���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pList->rpush("foo", values));
}

TEST_F(redis_list_test, rpushx)
{
    std::vector<std::string> result;

    // key���ڵ����
    EXPECT_EQ(4, redis_list_test::m_pList->rpushx("foo", "four"));
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, two, one, four", redis_helper::join(result, ", "));

    // key�����ڵ����
    redis_list_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pList->rpushx("foo", "four"));

    // key���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pList->rpushx("foo", "four"));
}

TEST_F(redis_list_test, blpop_non_block)
{
    std::vector<std::string> keys;
    keys.push_back("foo");
    std::string key, value;

    // �����б��е�һ��Ԫ��
    EXPECT_EQ(1, redis_list_test::m_pList->blpop(keys, 2, key, value));
    EXPECT_EQ("foo", key);
    EXPECT_EQ("three", value);

    // keyΪ���б�, ��ʱ�����
    redis_list_test::m_pList->ltrim("foo", 1, 0);
    EXPECT_EQ(0, redis_list_test::m_pList->blpop(keys, 2, key, value));

    // key���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pList->blpop(keys, 2, key, value));

    // ��������(��ʱʱ��Ϊ��ֵ) 
    EXPECT_EQ(-1, redis_list_test::m_pList->blpop(keys, -2, key, value));
}

TEST_F(redis_list_test, brpop_non_block)
{
    std::vector<std::string> keys;
    keys.push_back("foo");
    std::string key, value;

    // �����б�ĩβԪ��
    EXPECT_EQ(1, redis_list_test::m_pList->brpop(keys, 2, key, value));
    EXPECT_EQ("foo", key);
    EXPECT_EQ("one", value);

    // keyΪ���б�, ��ʱ�����
    redis_list_test::m_pList->ltrim("foo", 1, 0);
    EXPECT_EQ(0, redis_list_test::m_pList->brpop(keys, 2, key, value));

    // key���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pList->brpop(keys, 2, key, value));

    // ��������(��ʱʱ��Ϊ��ֵ) 
    EXPECT_EQ(-1, redis_list_test::m_pList->brpop(keys, -2, key, value));
}

TEST_F(redis_list_test, brpoplpush_non_block)
{
    std::string result;
    std::vector<std::string> vaules;

    // ��srcĩβ����һ��Ԫ�ز�����destͷ��
    EXPECT_EQ(1, redis_list_test::m_pList->brpoplpush("foo", "{foo}:1", 2, result));
    EXPECT_EQ("one", result);
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("three, two", redis_helper::join(vaules, ", "));
    vaules.clear();
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("{foo}:1", 0, -1, vaules));
    EXPECT_EQ("one", redis_helper::join(vaules, ", "));
    vaules.clear();

    // src�����ڵ����
    redis_list_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pList->brpoplpush("foo", "{foo}:1", 2, result));

    // src���ʹ�������
    redis_list_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pList->brpoplpush("foo", "{foo}:1", 2, result));
    
    // ��������(��ʱʱ��Ϊ��ֵ)
    EXPECT_EQ(-1, redis_list_test::m_pList->brpoplpush("foo", "{foo}:1", -2, result));

    redis_list_test::m_pKey->del("{foo}:1");
}

TEST_F(redis_list_test, brpoplpush_src_same_with_dest)
{
    std::string result;
    std::vector<std::string> vaules;

    // ��srcĩβ����һ��Ԫ�ز�����destͷ��
    EXPECT_EQ(1, redis_list_test::m_pList->brpoplpush("foo", "foo", 2, result));
    EXPECT_EQ("one", result);
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("one, three, two", redis_helper::join(vaules, ", "));

    // dest���ʹ�������
    redis_list_test::m_pStr->set("{foo}:1", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pList->brpoplpush("foo", "{foo}:1", 2, result));

    redis_list_test::m_pKey->del("{foo}:1");
}

// ģ������һ���ͻ��˲�������
void* other_client(void*)
{
    sleep(5);
    
    redis_client* pClient =
        new redis_client(gt_component::Instance().get_cluster_host(),
                         gt_component::Instance().get_cluster_port());
    redis_list* pList = new redis_list(pClient);

    std::vector<std::string> values;
    values.push_back("one");
    pList->lpush("foo", values);

    delete pClient;
    delete pList;

    return 0;
}

TEST_F(redis_list_test, blpop_with_block)
{
    std::vector<std::string> keys;
    keys.push_back("foo");

    redis_list_test::m_pList->ltrim("foo", 1, 0);
    // ����һ���߳�ģ������һ��redis client
    pthread_t id;
    EXPECT_EQ(0, pthread_create(&id, NULL, other_client, NULL));

    // �����б��е�һ��Ԫ��
    std::string key, value;
    EXPECT_EQ(1, redis_list_test::m_pList->blpop(keys, 0, key, value));
    EXPECT_EQ("foo", key);
    EXPECT_EQ("one", value);

    pthread_join(id, NULL);
}

TEST_F(redis_list_test, brpop_with_block)
{
    std::vector<std::string> keys;
    keys.push_back("foo");

    redis_list_test::m_pList->ltrim("foo", 1, 0);
    // ����һ���߳�ģ������һ��redis client
    pthread_t id;
    EXPECT_EQ(0, pthread_create(&id, NULL, other_client, NULL));

    // �����б�ĩβԪ��
    std::string key, value;
    EXPECT_EQ(1, redis_list_test::m_pList->brpop(keys, 0, key, value));
    EXPECT_EQ("foo", key);
    EXPECT_EQ("one", value);

    pthread_join(id, NULL);
}

TEST_F(redis_list_test, brpoplpush_with_block)
{
    std::string result;
    std::vector<std::string> vaules;

    redis_list_test::m_pList->ltrim("foo", 1, 0);
    // ����һ���߳�ģ������һ��redis client
    pthread_t id;
    EXPECT_EQ(0, pthread_create(&id, NULL, other_client, NULL));

    // ��srcĩβ����һ��Ԫ�ز�����destͷ��
    EXPECT_EQ(1, redis_list_test::m_pList->brpoplpush("foo", "foo", 0, result));
    EXPECT_EQ("one", result);
    EXPECT_EQ(true, redis_list_test::m_pList->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("one", redis_helper::join(vaules, ", "));

    pthread_join(id, NULL);
}

