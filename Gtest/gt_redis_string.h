#ifndef __GT_REDIS_STRING_H__
#define __GT_REDIS_STRING_H__

#include "gtest/gtest.h"
#include "redis_string.h"
#include "redis_client.h"

class redis_string;

class redis_string_test : public testing::Test
{
public:
    static void SetUpTestCase() {
        m_pClient = new redis_client("192.168.199.131", 10000);
    }
    static void TearDownTestCase() {
        delete m_pClient;
        m_pClient = NULL;
    }
//private:
    static redis_client* m_pClient;
};

#endif /* __GT_REDIS_STRING_H__ */
