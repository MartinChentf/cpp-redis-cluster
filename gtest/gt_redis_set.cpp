#include "gtest/gtest.h"
#include "gt_redis_set.h"
#include "gt_common.h"

#include "redis_client.h"
#include "redis_set.h"
#include "redis_key.h"
#include "redis_helper.h"
#include "redis_string.h"

redis_client* redis_set_test::m_pClient = NULL;
redis_string* redis_set_test::m_pStr = NULL;
redis_set* redis_set_test::m_pSet = NULL;
redis_key* redis_set_test::m_pKey = NULL;

void redis_set_test::SetUpTestCase() {
    m_pClient = new redis_client(gt_component::Instance().get_host(),
                                 gt_component::Instance().get_port());
    m_pSet = new redis_set(m_pClient);
    m_pKey = new redis_key(m_pClient);
    m_pStr = new redis_string(m_pClient);

    m_pKey->del("foo");
}

void redis_set_test::TearDownTestCase() {
    delete m_pClient;
    m_pClient = NULL;
    delete m_pSet;
    m_pSet = NULL;
    delete m_pKey;
    m_pKey = NULL;
    delete m_pStr;
    m_pStr = NULL;
}

void redis_set_test::SetUp()
{
    std::vector<std::string> member;
    member.push_back("one");
    member.push_back("two");
    member.push_back("three");

    m_pSet->sadd("foo", member);
}

void redis_set_test::TearDown()
{
    m_pKey->del("foo");
}

bool redis_set_test::contain_with(std::string str, std::vector<std::string>& search)
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

TEST_F(redis_set_test, smembers)
{
    std::vector<std::string> result;

    // ��ȡ���ϳ�Ա
    EXPECT_EQ(3, redis_set_test::m_pSet->smembers("foo", result));
    EXPECT_EQ(true, contain_with("one, two, three", result));

    // key������
    redis_set_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pSet->smembers("foo", result));

    // key�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->smembers("foo", result));
}

TEST_F(redis_set_test, sismember)
{
    // �ж�ָ����Ա�Ƿ����
    EXPECT_EQ(1, redis_set_test::m_pSet->sismember("foo", "one"));
    EXPECT_EQ(1, redis_set_test::m_pSet->sismember("foo", "two"));
    EXPECT_EQ(0, redis_set_test::m_pSet->sismember("foo", "four"));

    // key������
    redis_set_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pSet->sismember("foo", "one"));

    // key�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->sismember("foo", "one"));
}

TEST_F(redis_set_test, sadd)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");

    // ����³�Ա
    EXPECT_EQ(1, redis_set_test::m_pSet->sadd("foo", member));
    EXPECT_EQ(1, redis_set_test::m_pSet->sismember("foo", "four"));
    EXPECT_EQ(1, redis_set_test::m_pSet->sismember("foo", "two"));

    // key�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->sadd("foo", member));
}

TEST_F(redis_set_test, scard)
{
    // ���ؼ��ϳ�Ա����
    EXPECT_EQ(3, redis_set_test::m_pSet->scard("foo"));

    // key������
    redis_set_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pSet->scard("foo"));

    // key�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->scard("foo"));
}

TEST_F(redis_set_test, sdiff)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");
    redis_set_test::m_pSet->sadd("{foo}:1", member);

    std::vector<std::string> result;
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");

    // ���ؼ���֮��
    EXPECT_EQ(true, redis_set_test::m_pSet->sdiff(keys, result));
    EXPECT_EQ(true, contain_with("one, three", result));
    result.clear();

    // �����key������
    redis_set_test::m_pKey->del("{foo}:1");
    EXPECT_EQ(true, redis_set_test::m_pSet->sdiff(keys, result));
    EXPECT_EQ(true, contain_with("one, two, three", result));
    result.clear();

    // ��һ��key������
    redis_set_test::m_pKey->del("foo");
    EXPECT_EQ(true, redis_set_test::m_pSet->sdiff(keys, result));
    EXPECT_EQ("", redis_helper::join(result, ", "));
    result.clear();

    // key�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(false, redis_set_test::m_pSet->sdiff(keys, result));

    redis_set_test::m_pKey->del("{foo}:1");
}

TEST_F(redis_set_test, sinter)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");
    redis_set_test::m_pSet->sadd("{foo}:1", member);

    std::vector<std::string> result;
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");

    // ���ؼ���֮�佻��
    EXPECT_EQ(true, redis_set_test::m_pSet->sinter(keys, result));
    EXPECT_EQ("two", redis_helper::join(result, ", "));
    result.clear();

    // �е�keyΪ��
    redis_set_test::m_pKey->del("{foo}:1");
    EXPECT_EQ(true, redis_set_test::m_pSet->sinter(keys, result));
    EXPECT_EQ("", redis_helper::join(result, ", "));
    result.clear();

    // key�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(false, redis_set_test::m_pSet->sinter(keys, result));

    redis_set_test::m_pKey->del("{foo}:1");
}

TEST_F(redis_set_test, sunion)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");
    redis_set_test::m_pSet->sadd("{foo}:1", member);

    std::vector<std::string> result;
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");

    // ���ؼ���֮�䲢��
    EXPECT_EQ(true, redis_set_test::m_pSet->sunion(keys, result));
    EXPECT_EQ(true, contain_with("one, two, three, four", result));
    result.clear();

    // �е�keyΪ��
    redis_set_test::m_pKey->del("{foo}:1");
    EXPECT_EQ(true, redis_set_test::m_pSet->sunion(keys, result));
    EXPECT_EQ(true, contain_with("one, two, three", result));
    result.clear();

    // key�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(false, redis_set_test::m_pSet->sunion(keys, result));

    redis_set_test::m_pKey->del("{foo}:1");
}

TEST_F(redis_set_test, sdiffstore)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");
    redis_set_test::m_pSet->sadd("{foo}:1", member);

    std::vector<std::string> result;
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");

    // ���ؼ���֮��
    EXPECT_EQ(2, redis_set_test::m_pSet->sdiffstore("{foo}:2", keys));
    EXPECT_EQ(2, redis_set_test::m_pSet->smembers("{foo}:2", result));
    EXPECT_EQ(true, contain_with("one, three", result));
    result.clear();

    // �����key������
    redis_set_test::m_pKey->del("{foo}:1");
    EXPECT_EQ(3, redis_set_test::m_pSet->sdiffstore("{foo}:2", keys));
    EXPECT_EQ(3, redis_set_test::m_pSet->smembers("{foo}:2", result));
    EXPECT_EQ(true, contain_with("one, two, three", result));
    result.clear();

    // ��һ��key������
    redis_set_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pSet->sdiffstore("{foo}:2", keys));

    // key�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->sdiffstore("{foo}:2", keys));

    redis_set_test::m_pKey->del("{foo}:1");
    redis_set_test::m_pKey->del("{foo}:2");
}

TEST_F(redis_set_test, sinterstore)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");
    redis_set_test::m_pSet->sadd("{foo}:1", member);

    std::vector<std::string> result;
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");

    // ���ؼ���֮�佻��
    EXPECT_EQ(1, redis_set_test::m_pSet->sinterstore("{foo}:2", keys));
    EXPECT_EQ(1, redis_set_test::m_pSet->smembers("{foo}:2", result));
    EXPECT_EQ("two", redis_helper::join(result, ", "));
    result.clear();

    // �е�keyΪ��
    redis_set_test::m_pKey->del("{foo}:1");
    EXPECT_EQ(0, redis_set_test::m_pSet->sinterstore("{foo}:2", keys));

    // key�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->sinterstore("{foo}:2", keys));

    redis_set_test::m_pKey->del("{foo}:1");
    redis_set_test::m_pKey->del("{foo}:2");
}

TEST_F(redis_set_test, sunionstore)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");
    redis_set_test::m_pSet->sadd("{foo}:1", member);

    std::vector<std::string> result;
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");

    // ���ؼ���֮�䲢��
    EXPECT_EQ(4, redis_set_test::m_pSet->sunionstore("{foo}:2", keys));
    EXPECT_EQ(4, redis_set_test::m_pSet->smembers("{foo}:2", result));
    EXPECT_EQ(true, contain_with("one, two, three, four", result));
    result.clear();

    // �е�keyΪ��
    redis_set_test::m_pKey->del("{foo}:1");
    EXPECT_EQ(3, redis_set_test::m_pSet->sunionstore("{foo}:2", keys));
    EXPECT_EQ(3, redis_set_test::m_pSet->smembers("{foo}:2", result));
    EXPECT_EQ(true, contain_with("one, two, three", result));
    result.clear();

    // key�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->sunionstore("{foo}:2", keys));

    redis_set_test::m_pKey->del("{foo}:1");
    redis_set_test::m_pKey->del("{foo}:2");
}

TEST_F(redis_set_test, smove)
{
    // �ƶ�ָ��Ԫ��
    EXPECT_EQ(1, redis_set_test::m_pSet->smove("foo", "{foo}:1", "two"));
    EXPECT_EQ(0, redis_set_test::m_pSet->sismember("foo", "two"));
    EXPECT_EQ(1, redis_set_test::m_pSet->sismember("{foo}:1", "two"));

    // ָ��Ԫ�ز�����
    EXPECT_EQ(0, redis_set_test::m_pSet->smove("foo", "{foo}:1", "two"));

    // src������
    redis_set_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pSet->smove("foo", "{foo}:1", "two"));

    // dest�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->smove("foo", "{foo}:1", "one"));

    redis_set_test::m_pKey->del("{foo}:1");
}

TEST_F(redis_set_test, spop)
{
    std::string result;

    // �������һ��Ԫ��
    EXPECT_EQ(1, redis_set_test::m_pSet->spop("foo", result));

    // key������
    redis_set_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pSet->spop("foo", result));

    // key�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->spop("foo", result));
}

TEST_F(redis_set_test, spop_with_count)
{
    std::vector<std::string> result;

    // �������count��Ԫ��
    EXPECT_EQ(2, redis_set_test::m_pSet->spop("foo", result, 2));

    // key������
    redis_set_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pSet->spop("foo", result, 2));

    // countΪ��ֵ
    EXPECT_EQ(-1, redis_set_test::m_pSet->spop("foo", result, -2));

    // key�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->spop("foo", result, 2));
}

TEST_F(redis_set_test, srandmember)
{
    std::string result;

    // �������һ��Ԫ��
    EXPECT_EQ(1, redis_set_test::m_pSet->srandmember("foo", result));

    // key������
    redis_set_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pSet->srandmember("foo", result));

    // key�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->srandmember("foo", result));
}

TEST_F(redis_set_test, srandmember_with_count)
{
    std::vector<std::string> result;

    // �������count��Ԫ��
    EXPECT_EQ(2, redis_set_test::m_pSet->srandmember("foo", result, 2));
    result.clear();
    
    // ��������Ԫ��(count����Ԫ�ظ���)
    EXPECT_EQ(3, redis_set_test::m_pSet->srandmember("foo", result, 10));
    EXPECT_EQ(true, contain_with("one, two, three", result));
    result.clear();

    // countΪ��ֵ
    EXPECT_EQ(5, redis_set_test::m_pSet->srandmember("foo", result, -5));
    EXPECT_EQ(true, contain_with("one, two, three", result));
    result.clear();

    // count = 0
    EXPECT_EQ(0, redis_set_test::m_pSet->srandmember("foo", result, 0));

    // key������
    redis_set_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pSet->srandmember("foo", result, 2));

    // key�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->srandmember("foo", result, 2));
}

TEST_F(redis_set_test, srem)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");

    // �Ƴ�ָ��Ԫ��
    EXPECT_EQ(1, redis_set_test::m_pSet->srem("foo", member));
    EXPECT_EQ(0, redis_set_test::m_pSet->sismember("foo", "two"));

    // ָ��Ԫ�ز�����
    EXPECT_EQ(0, redis_set_test::m_pSet->srem("foo", member));

    // key������
    redis_set_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pSet->srem("foo", member));

    // key�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->srem("foo", member));
}

TEST_F(redis_set_test, sscan)
{
    std::vector<std::string> result;

    // ��������(��Ϊ��ϣ��Ƚ�С, ����һ�ε��ü��ɱ���������ϣ��)
    EXPECT_EQ(0, redis_set_test::m_pSet->sscan("foo", 0, result));
    EXPECT_EQ(true, contain_with("one, two, three", result));
    result.clear();

    // ʹ��ģʽ����
    EXPECT_EQ(0, redis_set_test::m_pSet->sscan("foo", 0, result, "*e*"));
    EXPECT_EQ(true, contain_with("one, three", result));
    result.clear();

    // key�����ʹ���
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->sscan("foo", 0, result, "*e*"));
}

