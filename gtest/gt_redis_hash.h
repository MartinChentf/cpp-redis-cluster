#ifndef __GT_REDIS_HASH_H__
#define __GT_REDIS_HASH_H__

#include "gtest/gtest.h"

class redis;

class redis_hash_test : public testing::Test
{
public:
    static void SetUpTestCase();
    static void TearDownTestCase();

protected:
    virtual void SetUp();
    virtual void TearDown();

protected:    
    static redis* m_pRedis;
};

#endif /* __GT_REDIS_HASH_H__ */
