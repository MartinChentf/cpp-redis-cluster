#ifndef __GT_REDIS_LIST_H__
#define __GT_REDIS_LIST_H__

#include "gtest/gtest.h"

class redis;

class redis_list_test : public testing::Test
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

#endif /* __GT_REDIS_LIST_H__ */
