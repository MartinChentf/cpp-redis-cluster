#ifndef __GT_REDIS_HELPER_H__
#define __GT_REDIS_HELPER_H__

#include "gtest/gtest.h"

class redis_helper_test : public testing::Test
{
public:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}

protected:
    virtual void SetUp(){}
    virtual void TearDown(){}
};


#endif /* __GT_REDIS_HELPER_H__ */
