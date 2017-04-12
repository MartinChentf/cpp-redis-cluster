#ifndef __GT_REDIS_LIST_H__
#define __GT_REDIS_LIST_H__

#include "gtest/gtest.h"

class redis_client;
class redis_string;
class redis_list;
class redis_key;

class redis_list_test : public testing::Test
{
public:
    static void SetUpTestCase();
    static void TearDownTestCase();

protected:
    virtual void SetUp();
    virtual void TearDown();

protected:    
    static redis_string*    m_pStr;
    static redis_list*      m_pList;
    static redis_key*       m_pKey;

private:
    static redis_client* m_pClient;
};

#endif /* __GT_REDIS_LIST_H__ */
