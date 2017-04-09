#ifndef __GT_REDIS_HASH_H__
#define __GT_REDIS_HASH_H__

#include "gtest/gtest.h"

class redis_client;
class redis_string;
class redis_hash;
class redis_key;

class redis_hash_test : public testing::Test
{
public:
    static void SetUpTestCase();
    static void TearDownTestCase();

protected:
    virtual void SetUp();
    virtual void TearDown();

protected:    
    static redis_string*    m_pStr;
    static redis_hash*      m_pHash;
    static redis_key*       m_pKey;

private:
    static redis_client* m_pClient;
};

#endif /* __GT_REDIS_HASH_H__ */
