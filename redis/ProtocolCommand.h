/**
 * Copyright 2017-2022 chen.tengfei <chentf1836@163.com>
 * All rights reserved.
 *
 * @file   ProtocolCommand.h
 * @brief 
 * @version 1.0
 * @bug none
 * @warning none
 * @other none
 * @author chen.tengfei
 * @data   2017-06-07 22:22:48
 */
#ifndef __PROTOCOL_COMMAND_H__
#define __PROTOCOL_COMMAND_H__

#define CONVERT2STR(str) #str

enum BITOP { AND, OR, NOT, XOR };
#define BITOP_AND   CONVERT2STR(AND)
#define BITOP_OR    CONVERT2STR(OR)
#define BITOP_NOT   CONVERT2STR(NOT)
#define BITOP_XOR   CONVERT2STR(XOR)

enum CLIENT_TYPE { NORMAL, MASTER, SLAVE, PUBSUB };
#define TYPE_NORMAL CONVERT2STR(NORMAL)
#define TYPE_MASTER CONVERT2STR(MASTER)
#define TYPE_SLAVE  CONVERT2STR(SLAVE)
#define TYPE_PUBSUB CONVERT2STR(PUBSUB)

#endif /* __PROTOCOL_COMMAND_H__ */

