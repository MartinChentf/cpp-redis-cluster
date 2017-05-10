/**
 * Copyright 2017-2022 chen.tengfei <chentf1836@163.com>
 * All rights reserved.
 *
 * @brief 
 * @version 1.0
 * @bug none
 * @warning none
 * @other none
 * @author chen.tengfei
 * @data   2017-05-10 16:55:19
 */
#ifndef __REDIS_H__
#define __REDIS_H__

class redis
    : public redis_hash
    , publid redis_key
    , public redis_list
    , public redis_server
    , public redis_set
    , public redis_string
    , public redis_zset
{
public:
    redis(const std::string host, uint16_t port);
};

#endif /* __REDIS_H__ */

