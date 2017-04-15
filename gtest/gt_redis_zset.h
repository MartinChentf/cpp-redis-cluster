#ifndef __GT_REDIS_ZSET_H__
#define __GT_REDIS_ZSET_H__

#include "gtest/gtest.h"

class redis_client;
class redis_string;
class redis_zset;
class redis_key;

class redis_zset_test : public testing::Test
{
public:
    static void SetUpTestCase();
    static void TearDownTestCase();

protected:
    virtual void SetUp();
    virtual void TearDown();

    // for zinterstore and zunionstore
    void zstoreSetUp(std::vector<std::string>& keys,
                     std::vector<double>& weights);    
    void zstoreTearDown();

    void initializeSetUp();
    void allMemberSameScoreSetUp();

protected:    
    static redis_string*    m_pStr;
    static redis_zset*      m_pZset;
    static redis_key*       m_pKey;

private:
    static redis_client* m_pClient;
};

#endif /* __GT_REDIS_ZSET_H__ */
