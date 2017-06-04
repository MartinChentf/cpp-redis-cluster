#ifndef __GT_REDIS_KEY_H__
#define __GT_REDIS_KEY_H__

#include "gtest/gtest.h"

class redis;

class redis_key_test : public testing::Test
{
public:
    static void SetUpTestCase();
    static void TearDownTestCase();

protected:
    virtual void SetUp();
    virtual void TearDown();
    
    static bool contain_with(std::string str,
                             std::vector<std::string>& search);

protected:    
    static redis* m_pRedis;
};

#endif /* __GT_REDIS_KEY_H__ */
