#ifndef __GT_REDIS_SET_H__
#define __GT_REDIS_SET_H__

#include "gtest/gtest.h"

class redis_client;
class redis_string;
class redis_set;
class redis_key;

class redis_set_test : public testing::Test
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
    static redis_string*    m_pStr;
    static redis_set*       m_pSet;
    static redis_key*       m_pKey;

private:
    static redis_client* m_pClient;
};

#endif /* __GT_REDIS_SET_H__ */
