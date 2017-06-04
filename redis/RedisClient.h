/**
 * Copyright 2017-2022 chen.tengfei <chentf1836@163.com>
 * All rights reserved.
 *
 * @file   RedisClient.h
 * @brief 
 * @version 1.0
 * @bug none
 * @warning none
 * @other none
 * @author chen.tengfei
 * @data   2017-05-20 15:00:26
 */
#ifndef __REDIS_CLIENT_H__
#define __REDIS_CLIENT_H__

#include <stdint.h>
#include <string>

#include "redis_connection.h"
#include "redis_hash.h"
#include "redis_key.h"
#include "redis_list.h"
#include "redis_server.h"
#include "redis_set.h"
#include "redis_string.h"
#include "redis_zset.h"

class RedisClient
    : public redis_connection
    , public redis_hash
    , public redis_key
    , public redis_list
    , public redis_server
    , public redis_set
    , public redis_string
    , public redis_zset
{
public:
    RedisClient(const std::string& host, uint16_t port);
    ~RedisClient(){};
};

#endif /* __REDIS_CLIENT_H__ */

