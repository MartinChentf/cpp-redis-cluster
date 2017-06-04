#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "gt_redis_zset.h"
#include "gt_common.h"

#include "redis_helper.h"
#include "redis.h"

#define PUSH_BACK_PAIR(vector, first, second) \
    (vector).push_back(std::make_pair((first), (second)))

redis* redis_zset_test::m_pRedis = NULL;

void redis_zset_test::SetUpTestCase() {
    m_pRedis = new redis(gt_component::Instance().get_host(0),
                         gt_component::Instance().get_port(0));

    m_pRedis->del("foo");
}

void redis_zset_test::TearDownTestCase() {
    delete m_pRedis;
    m_pRedis = NULL;
}

void redis_zset_test::SetUp()
{
    initializeSetUp();
}

void redis_zset_test::TearDown()
{
    m_pRedis->del("foo");
}

void redis_zset_test::initializeSetUp()
{
    std::vector< std::pair<std::string, double> > member_score;
    PUSH_BACK_PAIR(member_score, "one", 1);
    PUSH_BACK_PAIR(member_score, "two", 2);
    PUSH_BACK_PAIR(member_score, "three", 3);
    PUSH_BACK_PAIR(member_score, "four", 4);

    m_pRedis->zadd("foo", member_score);
}

void redis_zset_test::allMemberSameScoreSetUp()
{
    std::vector< std::pair<std::string, double> > member_score;
    PUSH_BACK_PAIR(member_score, "one", 0);
    PUSH_BACK_PAIR(member_score, "two", 0);
    PUSH_BACK_PAIR(member_score, "three", 0);
    PUSH_BACK_PAIR(member_score, "four", 0);

    m_pRedis->zadd("foo", member_score);
}

void redis_zset_test::zstoreSetUp(std::vector<std::string>& keys,
                                  std::vector<double>& weights)
{
    std::vector< std::pair<std::string, double> > member_score;
    PUSH_BACK_PAIR(member_score, "two", 4);
    PUSH_BACK_PAIR(member_score, "four", 16);    
    PUSH_BACK_PAIR(member_score, "six", 36);
    redis_zset_test::m_pRedis->zadd("{foo}:1", member_score);

    keys.push_back("foo");
    keys.push_back("{foo}:1");
    weights.push_back(1);
    weights.push_back(0.5);
}

void redis_zset_test::zstoreTearDown()
{
    redis_zset_test::m_pRedis->del("{foo}:1");
    redis_zset_test::m_pRedis->del("{foo}:2");
}

TEST_F(redis_zset_test, zrange)
{
    std::vector<std::string> result;

    // ��ȡ�������г�Ա
    EXPECT_EQ(4, redis_zset_test::m_pRedis->zrange("foo", 0, -1, result));
    EXPECT_EQ("one, two, three, four", redis_helper::join(result, ", "));
    result.clear();

    // ��ȡ���򼯲��ֳ�Ա
    EXPECT_EQ(2, redis_zset_test::m_pRedis->zrange("foo", 1, 2, result));
    EXPECT_EQ("two, three", redis_helper::join(result, ", "));
    result.clear();

    // start > stop
    EXPECT_EQ(0, redis_zset_test::m_pRedis->zrange("foo", 3, 1, result));
    EXPECT_EQ("", redis_helper::join(result, ", "));
    result.clear();

    // stop���ڼ���Ԫ�ظ���
    EXPECT_EQ(3, redis_zset_test::m_pRedis->zrange("foo", 1, 10, result));
    EXPECT_EQ("two, three, four", redis_helper::join(result, ", "));
    result.clear();

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zrange("foo", 1, 3, result));
}

TEST_F(redis_zset_test, zrange_with_scores)
{
    std::vector< std::pair<std::string, double> > result;

    // ��ȡ�������г�Ա/����
    EXPECT_EQ(4, redis_zset_test::m_pRedis->zrange_with_scores("foo", 0, -1, result));
    EXPECT_EQ("one:1, two:2, three:3, four:4", redis_helper::join(result, ", "));
    result.clear();

    // ��ȡ���򼯲��ֳ�Ա/����
    EXPECT_EQ(3, redis_zset_test::m_pRedis->zrange_with_scores("foo", 1, 3, result));
    EXPECT_EQ("two:2, three:3, four:4", redis_helper::join(result, ", "));
    result.clear();

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zrange_with_scores("foo", 1, 3, result));
}

TEST_F(redis_zset_test, zadd)
{
    std::vector< std::pair<std::string, double> > result;
    std::vector< std::pair<std::string, double> > member_score;
    PUSH_BACK_PAIR(member_score, "five", 5);
    PUSH_BACK_PAIR(member_score, "six", 6);

    // �����³�Ա
    EXPECT_EQ(2, redis_zset_test::m_pRedis->zadd("foo", member_score));
    EXPECT_EQ(6, redis_zset_test::m_pRedis->zrange_with_scores("foo", 0, -1, result));
    EXPECT_EQ("one:1, two:2, three:3, four:4, five:5, six:6",
              redis_helper::join(result, ", "));
    result.clear();

    // ���·���ֵ
    member_score.clear();
    PUSH_BACK_PAIR(member_score, "five", 55);
    PUSH_BACK_PAIR(member_score, "six", 66);
    EXPECT_EQ(0, redis_zset_test::m_pRedis->zadd("foo", member_score)); // û�������³�Ա
    EXPECT_EQ(6, redis_zset_test::m_pRedis->zrange_with_scores("foo", 0, -1, result));
    EXPECT_EQ("one:1, two:2, three:3, four:4, five:55, six:66",
              redis_helper::join(result, ", "));
    result.clear();

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zadd("foo", member_score));
}

TEST_F(redis_zset_test, zadd_xx)
{
    std::vector< std::pair<std::string, double> > result;
    std::vector< std::pair<std::string, double> > member_score;
    PUSH_BACK_PAIR(member_score, "two", 20);
    PUSH_BACK_PAIR(member_score, "five", 5);

    // ʹ��XX��ʶ(ֻ�������г�Ա)
    EXPECT_EQ(0, redis_zset_test::m_pRedis->zadd_xx("foo", member_score));
    EXPECT_EQ(4, redis_zset_test::m_pRedis->zrange_with_scores("foo", 0, -1, result));
    EXPECT_EQ("one:1, three:3, four:4, two:20",
              redis_helper::join(result, ", "));
    result.clear();

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zadd_xx("foo", member_score));
}

TEST_F(redis_zset_test, zadd_nx)
{
    std::vector< std::pair<std::string, double> > result;
    std::vector< std::pair<std::string, double> > member_score;
    PUSH_BACK_PAIR(member_score, "two", 20);
    PUSH_BACK_PAIR(member_score, "five", 5);

    // ʹ��NX��ʶ(ֻ�����³�Ա)
    EXPECT_EQ(1, redis_zset_test::m_pRedis->zadd_nx("foo", member_score));
    EXPECT_EQ(5, redis_zset_test::m_pRedis->zrange_with_scores("foo", 0, -1, result));
    EXPECT_EQ("one:1, two:2, three:3, four:4, five:5",
              redis_helper::join(result, ", "));
    result.clear();

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zadd_nx("foo", member_score));
}

TEST_F(redis_zset_test, zadd_ch)
{
    std::vector< std::pair<std::string, double> > result;
    std::vector< std::pair<std::string, double> > member_score;
    PUSH_BACK_PAIR(member_score, "two", 20);
    PUSH_BACK_PAIR(member_score, "five", 5);

    // �Ƿ��޸ķ���ֵ(���������͸���ֵ�ĳ�Ա����)
    EXPECT_EQ(2, redis_zset_test::m_pRedis->zadd_ch("foo", member_score));
    EXPECT_EQ(5, redis_zset_test::m_pRedis->zrange_with_scores("foo", 0, -1, result));
    EXPECT_EQ("one:1, three:3, four:4, five:5, two:20",
              redis_helper::join(result, ", "));
    result.clear();

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zadd_ch("foo", member_score));
}

TEST_F(redis_zset_test, zadd_incr)
{
    std::string result;

    // ��ָ����Ա�����ۼ�
    EXPECT_EQ(true, redis_zset_test::m_pRedis->zadd_incr("foo", 4, "two", result));
    EXPECT_EQ("6", result);
    EXPECT_EQ(true, redis_zset_test::m_pRedis->zadd_incr("foo", -12, "two", result));
    EXPECT_EQ("-6", result);
    EXPECT_EQ(true, redis_zset_test::m_pRedis->zadd_incr("foo", 5, "five", result));
    EXPECT_EQ("5", result);

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(false, redis_zset_test::m_pRedis->zadd_incr("foo", 2, "two", result));
}

TEST_F(redis_zset_test, zcard)
{
    // �������򼯺ϻ���
    EXPECT_EQ(4, redis_zset_test::m_pRedis->zcard("foo"));

    // key������
    redis_zset_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_zset_test::m_pRedis->zcard("foo"));

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zcard("foo"));
}

TEST_F(redis_zset_test, zcount)
{
    // ����ָ������Ԫ������
    EXPECT_EQ(2, redis_zset_test::m_pRedis->zcount("foo", 2, 3));
    EXPECT_EQ(0, redis_zset_test::m_pRedis->zcount("foo", 2, 1));
    EXPECT_EQ(3, redis_zset_test::m_pRedis->zcount("foo", 2, 10));

    EXPECT_EQ(4, redis_zset_test::m_pRedis->zcount("foo", "-inf", "+inf"));
    EXPECT_EQ(1, redis_zset_test::m_pRedis->zcount("foo", "2", "(3"));
    EXPECT_EQ(2, redis_zset_test::m_pRedis->zcount("foo", "3", "+inf"));

    // key������
    redis_zset_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_zset_test::m_pRedis->zcount("foo", 2, 3));

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zcount("foo", 2, 3));
}

TEST_F(redis_zset_test, zincrby)
{
    std::string result;

    // ��ָ����Ա�����ۼ�
    EXPECT_EQ(true, redis_zset_test::m_pRedis->zincrby("foo", 4, "two", &result));
    EXPECT_EQ("6", result);
    EXPECT_EQ(true, redis_zset_test::m_pRedis->zincrby("foo", -12, "two", &result));
    EXPECT_EQ("-6", result);
    EXPECT_EQ(true, redis_zset_test::m_pRedis->zincrby("foo", 5, "five", &result));
    EXPECT_EQ("5", result);

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(false, redis_zset_test::m_pRedis->zincrby("foo", 2, "two", &result));
}

TEST_F(redis_zset_test, zinterstore_SUM)
{
    std::vector< std::pair<std::string, double> > result;
    std::vector<std::string> keys;
    std::vector<double> weights;
    redis_zset_test::zstoreSetUp(keys, weights);

    EXPECT_EQ(2, redis_zset_test::m_pRedis->zinterstore("{foo}:2", keys, &weights));
    EXPECT_EQ(2, redis_zset_test::m_pRedis->zrange_with_scores("{foo}:2", 0, -1, result));
    EXPECT_EQ("two:4, four:12", redis_helper::join(result, ", "));
    result.clear();

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zinterstore("{foo}:2", keys, &weights));

    redis_zset_test::zstoreTearDown();
}

TEST_F(redis_zset_test, zinterstore_MIN)
{
    std::vector< std::pair<std::string, double> > result;
    std::vector<std::string> keys;
    std::vector<double> weights;
    redis_zset_test::zstoreSetUp(keys, weights);

    EXPECT_EQ(2, redis_zset_test::m_pRedis->zinterstore("{foo}:2", keys, &weights, "MIN"));
    EXPECT_EQ(2, redis_zset_test::m_pRedis->zrange_with_scores("{foo}:2", 0, -1, result));
    EXPECT_EQ("two:2, four:4", redis_helper::join(result, ", "));
    result.clear();

    redis_zset_test::zstoreTearDown();
}

TEST_F(redis_zset_test, zinterstore_MAX)
{
    std::vector< std::pair<std::string, double> > result;
    std::vector<std::string> keys;
    std::vector<double> weights;
    redis_zset_test::zstoreSetUp(keys, weights);

    EXPECT_EQ(2, redis_zset_test::m_pRedis->zinterstore("{foo}:2", keys, &weights, "MAX"));
    EXPECT_EQ(2, redis_zset_test::m_pRedis->zrange_with_scores("{foo}:2", 0, -1, result));
    EXPECT_EQ("two:2, four:8", redis_helper::join(result, ", "));
    result.clear();

    redis_zset_test::zstoreTearDown();
}

TEST_F(redis_zset_test, zunionstore_SUM)
{
    std::vector< std::pair<std::string, double> > result;
    std::vector<std::string> keys;
    std::vector<double> weights;
    redis_zset_test::zstoreSetUp(keys, weights);

    EXPECT_EQ(5, redis_zset_test::m_pRedis->zunionstore("{foo}:2", keys, &weights));
    EXPECT_EQ(5, redis_zset_test::m_pRedis->zrange_with_scores("{foo}:2", 0, -1, result));
    EXPECT_EQ("one:1, three:3, two:4, four:12, six:18", redis_helper::join(result, ", "));
    result.clear();

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zunionstore("{foo}:2", keys, &weights));

    redis_zset_test::zstoreTearDown();
}

TEST_F(redis_zset_test, zunionstore_MIN)
{
    std::vector< std::pair<std::string, double> > result;
    std::vector<std::string> keys;
    std::vector<double> weights;
    redis_zset_test::zstoreSetUp(keys, weights);

    EXPECT_EQ(5, redis_zset_test::m_pRedis->zunionstore("{foo}:2", keys, &weights, "MIN"));
    EXPECT_EQ(5, redis_zset_test::m_pRedis->zrange_with_scores("{foo}:2", 0, -1, result));
    EXPECT_EQ("one:1, two:2, three:3, four:4, six:18", redis_helper::join(result, ", "));
    result.clear();

    redis_zset_test::zstoreTearDown();
}

TEST_F(redis_zset_test, zunionstore_MAX)
{
    std::vector< std::pair<std::string, double> > result;
    std::vector<std::string> keys;
    std::vector<double> weights;
    redis_zset_test::zstoreSetUp(keys, weights);

    EXPECT_EQ(5, redis_zset_test::m_pRedis->zunionstore("{foo}:2", keys, &weights, "MAX"));
    EXPECT_EQ(5, redis_zset_test::m_pRedis->zrange_with_scores("{foo}:2", 0, -1, result));
    EXPECT_EQ("one:1, two:2, three:3, four:8, six:18", redis_helper::join(result, ", "));
    result.clear();

    redis_zset_test::zstoreTearDown();
}

TEST_F(redis_zset_test, zlexcount)
{
    redis_zset_test::allMemberSameScoreSetUp();

    // ����ָ��������Ԫ�ظ���
    EXPECT_EQ(3, redis_zset_test::m_pRedis->zlexcount("foo", "(four", "[two"));
    EXPECT_EQ(4, redis_zset_test::m_pRedis->zlexcount("foo", "-", "+"));

    // key������
    redis_zset_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_zset_test::m_pRedis->zlexcount("foo", "(four", "[two"));

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zlexcount("foo", "(four", "[two"));
}

TEST_F(redis_zset_test, zrangebylex)
{
    redis_zset_test::allMemberSameScoreSetUp();

    std::vector<std::string> result;

    // ����ָ��������Ԫ�ظ���
    EXPECT_EQ(3, redis_zset_test::m_pRedis->zrangebylex("foo", "(four", "[two", result));
    EXPECT_EQ("one, three, two", redis_helper::join(result, ", "));
    result.clear();

    EXPECT_EQ(4, redis_zset_test::m_pRedis->zrangebylex("foo", "-", "+", result));
    EXPECT_EQ("four, one, three, two", redis_helper::join(result, ", "));
    result.clear();

    EXPECT_EQ(2, redis_zset_test::m_pRedis->zrangebylex("foo", "-", "+", result, 1, 2));
    EXPECT_EQ("one, three", redis_helper::join(result, ", "));
    result.clear();

    // key������
    redis_zset_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_zset_test::m_pRedis->zrangebylex("foo", "(four", "[two", result));

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zrangebylex("foo", "(four", "[two", result));
}

TEST_F(redis_zset_test, zrangebyscore)
{
    std::vector<std::string> result;

    EXPECT_EQ(3, redis_zset_test::m_pRedis->zrangebyscore("foo", 1, 3, result));
    EXPECT_EQ("one, two, three", redis_helper::join(result, ", "));
    result.clear();

    EXPECT_EQ(2, redis_zset_test::m_pRedis->zrangebyscore("foo", "-inf", "(3", result));
    EXPECT_EQ("one, two", redis_helper::join(result, ", "));
    result.clear();

    EXPECT_EQ(1, redis_zset_test::m_pRedis->zrangebyscore("foo", "-inf", "+inf", result, 3, 2));
    EXPECT_EQ("four", redis_helper::join(result, ", "));
    result.clear();

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zrangebyscore("foo", 1, 3, result));
}

TEST_F(redis_zset_test, zrangebyscore_with_scores)
{
    std::vector< std::pair<std::string, double> > result;

    EXPECT_EQ(3, redis_zset_test::m_pRedis->zrangebyscore_with_scores("foo", 1, 3, result));
    EXPECT_EQ("one:1, two:2, three:3", redis_helper::join(result, ", "));
    result.clear();

    EXPECT_EQ(2, redis_zset_test::m_pRedis->zrangebyscore_with_scores("foo", "-inf", "(3", result));
    EXPECT_EQ("one:1, two:2", redis_helper::join(result, ", "));
    result.clear();

    EXPECT_EQ(1, redis_zset_test::m_pRedis->zrangebyscore_with_scores("foo", "-inf", "+inf", result, 3, 2));
    EXPECT_EQ("four:4", redis_helper::join(result, ", "));
    result.clear();

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zrangebyscore_with_scores("foo", 1, 3, result));
}

TEST_F(redis_zset_test, zrank)
{
    EXPECT_EQ(2, redis_zset_test::m_pRedis->zrank("foo", "three"));

    // member������
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zrank("foo", "five"));

    // key������
    redis_zset_test::m_pRedis->del("foo");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zrank("foo", "three"));

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zrank("foo", "three"));
}

TEST_F(redis_zset_test, zrem)
{
    std::vector<std::string> member;
    member.push_back("one");
    member.push_back("four");
    member.push_back("five");

    // ɾ��ָ��Ԫ��
    EXPECT_EQ(2, redis_zset_test::m_pRedis->zrem("foo", member));

    // member������
    EXPECT_EQ(0, redis_zset_test::m_pRedis->zrem("foo", member));

    // key������
    redis_zset_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_zset_test::m_pRedis->zrem("foo", member));

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zrem("foo", member));
}

TEST_F(redis_zset_test, zremrangebylex)
{
    std::vector<std::string> result;

    // �Ƴ�ָ��������Ԫ��
    redis_zset_test::allMemberSameScoreSetUp();
    EXPECT_EQ(3, redis_zset_test::m_pRedis->zremrangebylex("foo", "(four", "[two"));
    EXPECT_EQ(1, redis_zset_test::m_pRedis->zrange("foo", 0, -1, result));    
    EXPECT_EQ("four", redis_helper::join(result, ", "));
    result.clear();

    redis_zset_test::allMemberSameScoreSetUp();
    EXPECT_EQ(4, redis_zset_test::m_pRedis->zremrangebylex("foo", "-", "+"));
    EXPECT_EQ(0, redis_zset_test::m_pRedis->zrange("foo", 0, -1, result));    
    EXPECT_EQ("", redis_helper::join(result, ", "));
    result.clear();

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zremrangebylex("foo", "(four", "[two"));
}

TEST_F(redis_zset_test, zremrangebyrank)
{
    std::vector< std::pair<std::string, double> > result;

    // �Ƴ�ָ�������ڵ�Ԫ��(stop���ڼ��ϻ���)
    EXPECT_EQ(2, redis_zset_test::m_pRedis->zremrangebyrank("foo", 2, 4));
    EXPECT_EQ(2, redis_zset_test::m_pRedis->zrange_with_scores("foo", 0, -1, result));    
    EXPECT_EQ("one:1, two:2", redis_helper::join(result, ", "));
    result.clear();

    // start > stop
    redis_zset_test::initializeSetUp();
    EXPECT_EQ(0, redis_zset_test::m_pRedis->zremrangebyrank("foo", 4, 2));
    EXPECT_EQ(4, redis_zset_test::m_pRedis->zrange_with_scores("foo", 0, -1, result));    
    EXPECT_EQ("one:1, two:2, three:3, four:4", redis_helper::join(result, ", "));
    result.clear();

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zremrangebyrank("foo", 2, 4));
}

TEST_F(redis_zset_test, zremrangebyscore)
{
    std::vector< std::pair<std::string, double> > result;

    EXPECT_EQ(2, redis_zset_test::m_pRedis->zremrangebyscore("foo", 1.5, 3));
    EXPECT_EQ(2, redis_zset_test::m_pRedis->zrange_with_scores("foo", 0, -1, result));    
    EXPECT_EQ("one:1, four:4", redis_helper::join(result, ", "));
    result.clear();

    redis_zset_test::initializeSetUp();
    EXPECT_EQ(3, redis_zset_test::m_pRedis->zremrangebyscore("foo", "(1.5", "+inf"));
    EXPECT_EQ(1, redis_zset_test::m_pRedis->zrange_with_scores("foo", 0, -1, result));    
    EXPECT_EQ("one:1", redis_helper::join(result, ", "));
    result.clear();

    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zremrangebyscore("foo", "(1.5", "+inf"));
}

TEST_F(redis_zset_test, zrevrange)
{
    std::vector<std::string> result;

    // ��ȡ�������г�Ա
    EXPECT_EQ(4, redis_zset_test::m_pRedis->zrevrange("foo", 0, -1, result));
    EXPECT_EQ("four, three, two, one", redis_helper::join(result, ", "));
    result.clear();

    // ��ȡ���򼯲��ֳ�Ա
    EXPECT_EQ(2, redis_zset_test::m_pRedis->zrevrange("foo", 1, 2, result));
    EXPECT_EQ("three, two", redis_helper::join(result, ", "));
    result.clear();

    // start > stop
    EXPECT_EQ(0, redis_zset_test::m_pRedis->zrevrange("foo", 3, 1, result));
    EXPECT_EQ("", redis_helper::join(result, ", "));
    result.clear();

    // stop���ڼ���Ԫ�ظ���
    EXPECT_EQ(3, redis_zset_test::m_pRedis->zrevrange("foo", 1, 10, result));
    EXPECT_EQ("three, two, one", redis_helper::join(result, ", "));
    result.clear();

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zrevrange("foo", 1, 3, result));
}

TEST_F(redis_zset_test, zscan)
{
    std::vector< std::pair<std::string, double> > member_score;
    for (int i = 0; i < 26; i++) {
        char str[3] = {0};
        snprintf(str, sizeof(str), "%c%c", i + 'a', i + 'a');
        PUSH_BACK_PAIR(member_score, str, i + 5);
    }
    redis_zset_test::m_pRedis->zadd("foo", member_score);

    std::vector< std::pair<std::string, double> > result;
    // 1. ����
    int cursor = 0;
    do {
        cursor = redis_zset_test::m_pRedis->zscan("foo", cursor, result);
    } while (cursor > 0);
    EXPECT_EQ(30, result.size());
    std::string strTest = "one:1, two:2, three:3, four:4, "
        + redis_helper::join(member_score, ", ");
    EXPECT_EQ(strTest, redis_helper::join(result, ", "));
    result.clear();

    // ʹ�ò���ģʽ
    cursor = 0;
    do {
        cursor = redis_zset_test::m_pRedis->zscan("foo", cursor, result, "*e");
    } while (cursor > 0);
    EXPECT_EQ("one:1, three:3, ee:9", redis_helper::join(result, ", "));
    result.clear();

    // key�����ʹ���
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zscan("foo", 0, result));
}

TEST_F(redis_zset_test, zscore)
{
    std::string score;

    // ����ָ����Ա����ֵ
    EXPECT_EQ(1, redis_zset_test::m_pRedis->zscore("foo", "four", score));
    EXPECT_EQ("4", score);

    // ����ָ����Ա����ֵ
    EXPECT_EQ(0, redis_zset_test::m_pRedis->zscore("foo", "five", score));

    // ����ָ����Ա����ֵ
    redis_zset_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_zset_test::m_pRedis->zscore("foo", "four", score));

    // ����ָ����Ա����ֵ
    redis_zset_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_zset_test::m_pRedis->zscore("foo", "four", score));
}

