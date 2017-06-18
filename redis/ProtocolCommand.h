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

#define STRING(str) #str

enum BITOP { AND, OR, NOT, XOR };

enum CLIENT_TYPE { NORMAL, MASTER, SLAVE, PUBSUB };

enum CLIENT_REPLY { ON, OFF, SKIP };

enum SET_PARAM { EX, PX, NONE, NX, XX };

#endif /* __PROTOCOL_COMMAND_H__ */

