#include <pthread.h>

#include "gtest/gtest.h"
#include "gt_redis_list.h"
#include "gt_common.h"

#include "Util.h"
#include "redis.h"

redis* redis_list_test::m_pRedis = NULL;

void redis_list_test::SetUpTestCase() {
    m_pRedis = new redis(gt_component::Instance().get_host(0),
                         gt_component::Instance().get_port(0));

    m_pRedis->del("foo");
}

void redis_list_test::TearDownTestCase() {
    delete m_pRedis;
    m_pRedis = NULL;
}

void redis_list_test::SetUp()
{
    std::vector<std::string> values;
    values.push_back("one");
    values.push_back("two");
    values.push_back("three");

    redis_list_test::m_pRedis->lpush("foo", values);
}

void redis_list_test::TearDown()
{
    redis_list_test::m_pRedis->del("foo");
}

TEST_F(redis_list_test, lrange)
{
    std::vector<std::string> result;

    // ����ȫ��Ԫ��
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, two, one", Util::join(result, ", "));
    result.clear();

    // ���ز���Ԫ��
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 1, 2, result));
    EXPECT_EQ("two, one", Util::join(result, ", "));
    result.clear();

    // start > stop
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 2, 0, result));
    EXPECT_EQ("", Util::join(result, ", "));
    result.clear();

    // stop����ʵ���б�ĩβԪ���±�
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 1, 10, result));
    EXPECT_EQ("two, one", Util::join(result, ", "));
    result.clear();

    // key���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(false, redis_list_test::m_pRedis->lrange("foo", 1, 2, result));
}

TEST_F(redis_list_test, lpush)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("four");
    values.push_back("five");

    // ���б�ͷ��������Ԫ��
    EXPECT_EQ(5, redis_list_test::m_pRedis->lpush("foo", values));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("five, four, three, two, one", Util::join(result, ", "));
    
    // key���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->lpush("foo", values));
}

TEST_F(redis_list_test, lpushx)
{
    std::vector<std::string> result;

    // key���ڵ����
    EXPECT_EQ(4, redis_list_test::m_pRedis->lpushx("foo", "four"));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("four, three, two, one", Util::join(result, ", "));

    // key�����ڵ����
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->lpushx("foo", "four"));

    // key���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->lpushx("foo", "four"));
}

TEST_F(redis_list_test, lindex)
{
    std::string result;

    // 1. ��ȡindex����Ԫ��
    EXPECT_EQ(1, redis_list_test::m_pRedis->lindex("foo", 1, result));
    EXPECT_EQ("two", result);

    // 2. indexΪ��ֵ�����
    EXPECT_EQ(1, redis_list_test::m_pRedis->lindex("foo", -1, result));
    EXPECT_EQ("one", result);

    // 3. index������Χ�����
    EXPECT_EQ(0, redis_list_test::m_pRedis->lindex("foo", 3, result));

    // 4. key�����ڵ����(�൱��index������Χ)
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->lindex("foo", 0, result));
    
    // 5. key���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->lindex("foo", 0, result));
}

TEST_F(redis_list_test, linsert)
{
    std::vector<std::string> result;

    // ����ָ��Ԫ��֮ǰ
    EXPECT_EQ(4, redis_list_test::m_pRedis->linsert("foo", true, "two", "four"));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, four, two, one", Util::join(result, ", "));
    result.clear();

    // ����ָ��Ԫ��֮��
    EXPECT_EQ(5, redis_list_test::m_pRedis->linsert("foo", false, "two", "five"));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, four, two, five, one", Util::join(result, ", "));
    result.clear();

    // ָ��Ԫ�ز����ڵ����
    EXPECT_EQ(-1, redis_list_test::m_pRedis->linsert("foo", true, "six", "five"));

    // key�����ڵ����
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->linsert("foo", true, "two", "five"));

    // key���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->linsert("foo", true, "two", "four"));
}

TEST_F(redis_list_test, llen)
{
    // �����б���
    EXPECT_EQ(3, redis_list_test::m_pRedis->llen("foo"));

    // key�����ڵ����
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->llen("foo"));
    
    // key���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->llen("foo"));
}

TEST_F(redis_list_test, lpop)
{
    std::string result;

    // �����б��е�һ��Ԫ��
    EXPECT_EQ(1, redis_list_test::m_pRedis->lpop("foo", result));
    EXPECT_EQ("three", result);

    // key�����ڵ����
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->lpop("foo", result));
    
    // key���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->lpop("foo", result));
}

TEST_F(redis_list_test, lrem_from_head)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("two");
    values.push_back("one");
    values.push_back("two");
    redis_list_test::m_pRedis->lpush("foo", values);

    // ��ͷ����ʼ�Ƴ�
    EXPECT_EQ(2, redis_list_test::m_pRedis->lrem("foo", 2, "two"));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("one, three, two, one", Util::join(result, ", "));

    // vaule�����ڵ����
    EXPECT_EQ(0, redis_list_test::m_pRedis->lrem("foo", 2, "four"));

    // key�����ڵ����
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->lrem("foo", 2, "two"));

    // key���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->lrem("foo", 2, "two"));
}

TEST_F(redis_list_test, lrem_from_tail)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("two");
    values.push_back("one");
    values.push_back("two");
    redis_list_test::m_pRedis->lpush("foo", values);

    // ��β����ʼ�Ƴ�
    EXPECT_EQ(2, redis_list_test::m_pRedis->lrem("foo", -2, "two"));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("two, one, three, one", Util::join(result, ", "));
}

TEST_F(redis_list_test, lrem_all)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("two");
    values.push_back("one");
    values.push_back("two");
    redis_list_test::m_pRedis->lpush("foo", values);

    // �Ƴ�����
    EXPECT_EQ(3, redis_list_test::m_pRedis->lrem("foo", 0, "two"));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("one, three, one", Util::join(result, ", "));
}

TEST_F(redis_list_test, lset)
{
    std::vector<std::string> result;

    // ������ֵ
    EXPECT_EQ(true, redis_list_test::m_pRedis->lset("foo", 1, "four"));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, four, one", Util::join(result, ", "));

    // indexԽ������
    EXPECT_EQ(false, redis_list_test::m_pRedis->lset("foo", 3, "four"));
    
    // key�����ڵ����
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(false, redis_list_test::m_pRedis->lset("foo", 1, "four"));

    // key���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(false, redis_list_test::m_pRedis->lset("foo", 1, "four"));
}

TEST_F(redis_list_test, ltrim)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("four");
    values.push_back("five");
    values.push_back("six");
    values.push_back("seven");
    redis_list_test::m_pRedis->lpush("foo", values);

    EXPECT_EQ(true, redis_list_test::m_pRedis->ltrim("foo", 2, 5));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("five, four, three, two", Util::join(result, ", "));

    // key�����ڵ����
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(true, redis_list_test::m_pRedis->ltrim("foo", 2, 5));

    // key���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(false, redis_list_test::m_pRedis->ltrim("foo", 2, 5));
}

TEST_F(redis_list_test, ltrim_start_gt_stop)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("four");
    values.push_back("five");
    values.push_back("six");
    values.push_back("seven");
    redis_list_test::m_pRedis->lpush("foo", values);

    // start > stop
    EXPECT_EQ(true, redis_list_test::m_pRedis->ltrim("foo", 6, 3));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("", Util::join(result, ", "));
}

TEST_F(redis_list_test, ltrim_stop_gt_end)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("four");
    values.push_back("five");
    values.push_back("six");
    values.push_back("seven");
    redis_list_test::m_pRedis->lpush("foo", values);

    // stop�����б��±�
    EXPECT_EQ(true, redis_list_test::m_pRedis->ltrim("foo", 2, 10));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("five, four, three, two, one", Util::join(result, ", "));
}

TEST_F(redis_list_test, rpop)
{
    std::string result;

    // �����б��е�һ��Ԫ��
    EXPECT_EQ(1, redis_list_test::m_pRedis->rpop("foo", result));
    EXPECT_EQ("one", result);

    // key������
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->rpop("foo", result));
    
    // key���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->rpop("foo", result));
}

TEST_F(redis_list_test, rpoplpush)
{
    std::string result;
    std::vector<std::string> vaules;

    // ��srcĩβ����һ��Ԫ�ز�����destͷ��
    EXPECT_EQ(1, redis_list_test::m_pRedis->rpoplpush("foo", "{foo}1", result));
    EXPECT_EQ("one", result);
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("three, two", Util::join(vaules, ", "));
    vaules.clear();
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("three, two", Util::join(vaules, ", "));
    vaules.clear();

    // src�����ڵ����
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->rpoplpush("foo", "{foo}1", result));

    // src���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->rpoplpush("foo", "{foo}1", result));
}

TEST_F(redis_list_test, rpoplpush_src_same_with_dest)
{
    std::string result;
    std::vector<std::string> vaules;

    // ��srcĩβ����һ��Ԫ�ز�����destͷ��
    EXPECT_EQ(1, redis_list_test::m_pRedis->rpoplpush("foo", "foo", result));
    EXPECT_EQ("one", result);
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("one, three, two", Util::join(vaules, ", "));

    // dest���ʹ�������
    redis_list_test::m_pRedis->set("{foo}1", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->rpoplpush("foo", "{foo}1", result));

    redis_list_test::m_pRedis->del("{foo}1");
}

TEST_F(redis_list_test, rpush)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("four");
    values.push_back("five");

    // ���б�ĩβ������Ԫ��
    EXPECT_EQ(5, redis_list_test::m_pRedis->rpush("foo", values));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, two, one, four, five", Util::join(result, ", "));

    // key���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->rpush("foo", values));
}

TEST_F(redis_list_test, rpushx)
{
    std::vector<std::string> result;

    // key���ڵ����
    EXPECT_EQ(4, redis_list_test::m_pRedis->rpushx("foo", "four"));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, two, one, four", Util::join(result, ", "));

    // key�����ڵ����
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->rpushx("foo", "four"));

    // key���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->rpushx("foo", "four"));
}

TEST_F(redis_list_test, blpop_non_block)
{
    std::vector<std::string> keys;
    keys.push_back("foo");
    std::string key, value;

    // �����б��е�һ��Ԫ��
    EXPECT_EQ(1, redis_list_test::m_pRedis->blpop(keys, 2, key, value));
    EXPECT_EQ("foo", key);
    EXPECT_EQ("three", value);

    // keyΪ���б�, ��ʱ�����
    redis_list_test::m_pRedis->ltrim("foo", 1, 0);
    EXPECT_EQ(0, redis_list_test::m_pRedis->blpop(keys, 2, key, value));

    // key���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->blpop(keys, 2, key, value));

    // ��������(��ʱʱ��Ϊ��ֵ) 
    EXPECT_EQ(-1, redis_list_test::m_pRedis->blpop(keys, -2, key, value));
}

TEST_F(redis_list_test, brpop_non_block)
{
    std::vector<std::string> keys;
    keys.push_back("foo");
    std::string key, value;

    // �����б�ĩβԪ��
    EXPECT_EQ(1, redis_list_test::m_pRedis->brpop(keys, 2, key, value));
    EXPECT_EQ("foo", key);
    EXPECT_EQ("one", value);

    // keyΪ���б�, ��ʱ�����
    redis_list_test::m_pRedis->ltrim("foo", 1, 0);
    EXPECT_EQ(0, redis_list_test::m_pRedis->brpop(keys, 2, key, value));

    // key���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->brpop(keys, 2, key, value));

    // ��������(��ʱʱ��Ϊ��ֵ) 
    EXPECT_EQ(-1, redis_list_test::m_pRedis->brpop(keys, -2, key, value));
}

TEST_F(redis_list_test, brpoplpush_non_block)
{
    std::string result;
    std::vector<std::string> vaules;

    // ��srcĩβ����һ��Ԫ�ز�����destͷ��
    EXPECT_EQ(1, redis_list_test::m_pRedis->brpoplpush("foo", "{foo}:1", 2, result));
    EXPECT_EQ("one", result);
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("three, two", Util::join(vaules, ", "));
    vaules.clear();
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("{foo}:1", 0, -1, vaules));
    EXPECT_EQ("one", Util::join(vaules, ", "));
    vaules.clear();

    // src�����ڵ����
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->brpoplpush("foo", "{foo}:1", 2, result));

    // src���ʹ�������
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->brpoplpush("foo", "{foo}:1", 2, result));
    
    // ��������(��ʱʱ��Ϊ��ֵ)
    EXPECT_EQ(-1, redis_list_test::m_pRedis->brpoplpush("foo", "{foo}:1", -2, result));

    redis_list_test::m_pRedis->del("{foo}:1");
}

TEST_F(redis_list_test, brpoplpush_src_same_with_dest)
{
    std::string result;
    std::vector<std::string> vaules;

    // ��srcĩβ����һ��Ԫ�ز�����destͷ��
    EXPECT_EQ(1, redis_list_test::m_pRedis->brpoplpush("foo", "foo", 2, result));
    EXPECT_EQ("one", result);
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("one, three, two", Util::join(vaules, ", "));

    // dest���ʹ�������
    redis_list_test::m_pRedis->set("{foo}:1", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->brpoplpush("foo", "{foo}:1", 2, result));

    redis_list_test::m_pRedis->del("{foo}:1");
}

// ģ������һ���ͻ��˲�������
void* other_client(void*)
{
    sleep(5);
    
    redis* pRedis = new redis(gt_component::Instance().get_host(0),
                              gt_component::Instance().get_port(0));

    std::vector<std::string> values;
    values.push_back("one");
    pRedis->lpush("foo", values);

    delete pRedis;
    return 0;
}

TEST_F(redis_list_test, blpop_with_block)
{
    std::vector<std::string> keys;
    keys.push_back("foo");

    redis_list_test::m_pRedis->ltrim("foo", 1, 0);
    // ����һ���߳�ģ������һ��redis client
    pthread_t id;
    EXPECT_EQ(0, pthread_create(&id, NULL, other_client, NULL));

    // �����б��е�һ��Ԫ��
    std::string key, value;
    EXPECT_EQ(1, redis_list_test::m_pRedis->blpop(keys, 0, key, value));
    EXPECT_EQ("foo", key);
    EXPECT_EQ("one", value);

    pthread_join(id, NULL);
}

TEST_F(redis_list_test, brpop_with_block)
{
    std::vector<std::string> keys;
    keys.push_back("foo");

    redis_list_test::m_pRedis->ltrim("foo", 1, 0);
    // ����һ���߳�ģ������һ��redis client
    pthread_t id;
    EXPECT_EQ(0, pthread_create(&id, NULL, other_client, NULL));

    // �����б�ĩβԪ��
    std::string key, value;
    EXPECT_EQ(1, redis_list_test::m_pRedis->brpop(keys, 0, key, value));
    EXPECT_EQ("foo", key);
    EXPECT_EQ("one", value);

    pthread_join(id, NULL);
}

TEST_F(redis_list_test, brpoplpush_with_block)
{
    std::string result;
    std::vector<std::string> vaules;

    redis_list_test::m_pRedis->ltrim("foo", 1, 0);
    // ����һ���߳�ģ������һ��redis client
    pthread_t id;
    EXPECT_EQ(0, pthread_create(&id, NULL, other_client, NULL));

    // ��srcĩβ����һ��Ԫ�ز�����destͷ��
    EXPECT_EQ(1, redis_list_test::m_pRedis->brpoplpush("foo", "foo", 0, result));
    EXPECT_EQ("one", result);
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("one", Util::join(vaules, ", "));

    pthread_join(id, NULL);
}

