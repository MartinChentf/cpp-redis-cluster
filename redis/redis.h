/**
 * Copyright 2017-2022 chen.tengfei <chentf1836@163.com>
 * All rights reserved.
 *
 * @file   redis.h
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

#include <stdint.h>
#include <string>
#include <vector>
#include <map>

#include "RedisClient.h"

class redis
{
public:
    redis(const std::string host, uint16_t port);
    ~redis();

public:
    /***************************************************************************
     * string command
     ***************************************************************************/
    /**
     * @description
     *   ����key���������ַ�����ֵ
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [OUT] result {std::string&}
     *   ����key���������ַ���ֵ, ���key�����ڷ��ؿ��ַ���. ��ʱ��Ҫ���ݷ���ֵ
     *   �ж��Ƿ�key�Ƿ����
     * @return {int} ���ز������, ����ֵ����:
     *   1: key����, ���ɹ��������������ַ���ֵ
     *   0: key������, ���ؿ��ַ���
     *  -1: �����key���ַ�������
     */
    int get(const std::string& key, std::string& result);

    /**
     * @description
     *   ��key�������ַ���ֵ����Ϊvalue��������key�������ľ�ֵ
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] value {const std::string&} key���������ַ�����ֵ
     * @param [OUT] result {std::string&} key���������ַ�����ֵ�����key�����ڷ�
     *   �ؿ��ַ���. ��ʱ��Ҫ���ݷ���ֵ�ж��Ƿ�key�Ƿ����
     * @return {int} ���ز������, ����ֵ����:
     *   1: key����, ���ɹ��������������ַ���ֵ
     *   0: key������, ���ؿ��ַ���
     *  -1: �����key���ַ�������
     */
    int getSet(const std::string& key, const std::string& value,
        std::string& result);

    /**
     * @description
     *   ��key�������ַ���value, ���key����, ������ԭ����ֵ,
     *   ���Һ���ԭʼ����
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] value {const std::string&} key���������ַ���ֵ
     * @return {bool} �����Ƿ�ɹ�, �ò�������ʧ��
     */
    bool set(const std::string& key, const std::string& value);

    /**
     * @description
     *   ���key�����ڣ���key�������ַ���value
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] value {const std::string&} key���������ַ���ֵ
     * @return {bool} �����Ƿ�ɹ�, �ò�������ʧ��. ����ֵ����:
     *    true: key������
     *   false: keyδ������
     */
    bool setnx(const std::string& key, const std::string& value);

    /**
     * @description
     *   ���key���ڣ���key�������ַ���value, ��Ϊ�� setnx �෴
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] value {const std::string&} key���������ַ���ֵ
     * @return {bool} �����Ƿ�ɹ�, �ò�������ʧ��. ����ֵ����:
     *    true: key������
     *   false: keyδ������
     */
    bool setxx(const std::string& key, const std::string& value);

    /**
     * @description
     *   ��key�������ַ���value, ��������key��ָ����ʱ��second֮��ʱ
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] second {long long} ��ʱʱ��, ��λ:��
     * @param [IN] value {const std::string&} key���������ַ���ֵ
     * @return {bool} �����Ƿ�ɹ�, �ò�������ʧ��
     */
    bool setex(const std::string& key, long long second,
        const std::string& value);

    /**
     * @description
     *   ��key�������ַ���value, ��������key��ָ����ʱ��millisecond֮��ʱ
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] second {long long} ��ʱʱ��, ��λ:����
     * @param [IN] value {const std::string&} key���������ַ���ֵ
     * @return {bool} �����Ƿ�ɹ�, �ò�������ʧ��
     */
    bool psetex(const std::string& key, long long millisecond,
        const std::string& value);

    /**
     * @description
     *   ����key��Ӧ���ַ������Ӵ�, �Ӵ���λ����start��end����(���߶����Ӵ���)
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] start {int} �Ӵ���ʼλ��
     * @param [IN] end {int} �Ӵ�ĩβλ��
     * @param [OUT] result {std::string&} ����ָ�����Ӵ�
     * @return {bool} �����Ƿ�ɹ�, false��ʾ�����key���ַ�������
     */
    bool getrange(const std::string& key, int start, int end,
        std::string& result);

    /**
     * @description
     *   ����key����Ӧ���ַ����Ĳ���ֵ, ��ָ����offset����ʼ, ����value�ĳ���.
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] offset {int} ָ��ƫ��������ʼλ��, ���ƫ���������ַ�������,
     *  ����������0���
     * @param [IN] value {const std::string&} ���ǵ�ֵ
     * @param [OUT] length {long long *} ���ظ��Ǻ��ַ����ĳ���
     * @return {bool} �����Ƿ�ɹ�, false��ʾ�����key���ַ�������
     */
    bool setrange(const std::string& key, int offset,
        const std::string& value, long long* length = NULL);

    /**
     * @description
     *   ����key����Ӧ���ַ�����offset����bitֵ. ���ƫ���������ַ�������, ��
     *   �����ֱ��ٶ�Ϊ��0bits���������ռ�. ��key�����ڵ�ʱ��, ������Ϊ��һ��
     *   ���ַ���, ����offset���ǳ�����Χ, ���ַ������ٶ�Ϊ��0bits���������ռ�.
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] offset {int} ָ��bitλ��ƫ����.
     *   ��һ���ֽڵ������ЧλΪ0, �ڶ��ֽ������ЧλΪ8, ��������.
     *   setbit��bitpos��bitλƫ�����������.
     * @return {int} ����ָ������λ��ֵ, ����ֵ����:
     *   0��1: ��������
     *   -1: ����
     */
    int getbit(const std::string& key, int offset);

    /**
     * @description
     *   ���û����key����Ӧ���ַ�����offset����bitλ. ���ƫ���������ַ�������,
     *   ���ַ����ͱ��ٶ�Ϊ��0bits���������ռ�. ��key�����ڵ�ʱ��, ������Ϊ��
     *   һ�����ַ���, ����offset���ǳ�����Χ, ���ַ������ٶ�Ϊ��0bits��������
     *   �ռ�.
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] offset {int} ָ��bitλ��ƫ����. offset��Χ: [0, 2^32)
     * @param [IN] value {bool} ���û����offset����bitλ
     *   true: ����offset��bitλ
     *   false: ���offset��bitλ
     * @return {int} ����offset��ԭ����ֵ, ����ֵ����:
     *   0��1: ��������
     *   -1: ����
     */
    int setbit(const std::string& key, int offset, bool value);

    /**
     * @description
     *   ͳ��key����Ӧ���ַ�������Ϊ1��bitλ��, ��ָ��start��end, ��ֻͳ��
     *   [start, end]��������1��bitλ��
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] start {int} ��ʼ�ֽڵ�ƫ����
     * @param [IN] end {int} ĩβ�ֽڵ�ƫ����
     * @return {long long} ����Ϊ1��bitλ��
     *  >=0: ����Ϊ1��bitλ��
     *   -1: ����
     */
    long long bitcount(const std::string& key);
    long long bitcount(const std::string& key, int start, int end);

    /**
     * @description
     *   ��һ���������������λ���ַ���src_keys����λԪ����, ����������浽
     *   dest_key��
     * @param [IN] op {BITOP}
     *   λ������, ֻ��ʹ��ö��ֵ enum BITOP{ AND, OR, NOT, XOR}
     *   AND: ��һ������keyֵ���߼���, ����������浽dest_key��
     *    OR: ��һ������keyֵ���߼���, ����������浽dest_key��
     *   NOT: ��keyֵ�߼�ȡ��,  ����������浽dest_key��
     *   XOR: ��һ������keyֵ���߼����,  ����������浽dest_key��
     * @param [OUT] dest_key {const std::string&}
     *   ���������ַ�������
     * @param [IN] src_keys {const std::vector<std::string>&}
     *   ��Ҫ�����һ����һ���ַ�������
     * @return {long long} ���ر��浽dest_key���ֽڳ���
     *   >=0: ���浽dest_key���ֽڳ���
     *    -1: ����
     */
    long long bitop(BITOP op, const std::string& dest_key,
        const std::vector<std::string>& src_keys);
    long long bitop(BITOP op, const std::string& dest_key,
        const std::string& src_key); // ��src_key�İ汾

    /**
     * @description
     *   �����ַ��������һ��������Ϊvalue(0��1)��bitλ, ���ָ��start��end,
     *   ����[start, end]���䷶Χ�ڼ���, start��end��ָ�ַ������ֽڵ�ƫ����,
     *   ����bitλ��ƫ����
     * @param [IN] key {const std::string&} ���������ַ��������key
     * @param [IN] value {bool} ��������bitֵ
     *   false: ����bitֵΪ0��λ
     *    true: ����bitֵΪ1��λ
     * @param [IN] start {int}
     *   ��������ʼ�ֽڵ�ƫ����, Ĭ��ֵΪ0, ��ʾ��ͷ��ʼ����
     * @param [IN] end {int}
     *   ������ĩβ�ֽڵ�ƫ����, Ĭ��ֵΪ-1, ��ʾһֱ�������ַ���ĩβ
     * @return {long long} �����ַ��������һ��������Ϊvalue(0��1)��bitλ
     *   >=0: ��һ��������Ϊvalue(0��1)��bitλ
     *    -1: ����
     */
    long long bitpos(const std::string& key, bool value,
        int start = 0, int end = -1);

    /**
     * @description
     *   ��������ָ����key��������value, ���key��������ֵ����string����
     *   ��keyֵ�����ڶ�Ӧλ��Ϊ��ָ��.
     * @param [IN] vkeys {const std::vector<std::string>&}
     *   һ���ַ��������key
     * @param [OUT] result {std::vector<std::string*>&}
     *   keys��������value, ��Ҫ�������ֶ��ͷ��ڴ�
     * @return {bool} ����true��ʾִ�гɹ�, �õ��ò���ʧ��
     */
    bool mget(const std::vector<std::string>& vkeys,
        std::vector<std::string*>& result);

    /**
     * @description
     *   ��key��������Ӧ��value��, �Ḳ��ԭ�е�valueֵ, ��Ϊ��set����.
     * @param [IN] keyValues {const std::map<std::string, std::string>&} ��ֵ��
     * @return {bool} ����true��ʾִ�гɹ�, �õ��ò���ʧ��
     */
    bool mset(const std::map<std::string, std::string>& key_values);

    /**
     * @description
     *   ��key��������Ӧ��value��, ֻҪ��һ��key����, ����msetnx������ִ��.
     * @param [IN] keyValues {const std::map<std::string, std::string>&} ��ֵ��
     * @return {bool} ���ز����Ƿ�ɹ�, �õ��ò���ʧ��
     *    true: ��ʾ���е�key�����óɹ�
     *   false: û��key������(�Ѿ��д��ڵ�key)
     */
    bool msetnx(const std::map<std::string, std::string>& key_values);

    /**
     * @description
     *   �Դ洢��ָ��key����ִֵ�м�1����. ���key������, ����ǰ��key��ֵ��Ϊ0
     * @param [IN] key {const std::string&} ָ��������key
     * @param [OUT] result {long long*} �ǿ�ʱ�洢�������
     * @return {bool} ��ʾ���������Ƿ�ɹ�, ����ֵ����:
     *    true: �����ɹ�
     *   false: 1) key��value���ʹ���(non-string)
     *          2) key��value���ܱ�ʾ�����ֵ��ַ���
     */
    bool incr(const std::string& key, long long* result = NULL);

    /**
     * @description
     *   �Դ洢��ָ��key����ֵ����increment.
     *   ���key������, ����ǰ��key��ֵ��Ϊ0
     * @param [IN] key {const std::string&} ָ��������key
     * @param [IN] increment {long long} ����ֵ, <0��ʾ������
     * @param [OUT] result {long long*} �ǿ�ʱ�洢�������
     * @return {bool} ��ʾ���������Ƿ�ɹ�, ����ֵ����:
     *    true: �����ɹ�
     *   false: 1) key��value���ʹ���(non-string)
     *          2) key��value���ܱ�ʾ�����ֵ��ַ���
     */
    bool incrby(const std::string& key, long long increment,
        long long* result = NULL);

    /**
     * @description
     *   �Դ洢��ָ��key�ĸ�������ֵ����increment.
     *   ���key������, ����ǰ��key��ֵ��Ϊ0
     * @param [IN] key {const std::string&} ָ��������key
     * @param [IN] increment {double} ����ֵ, <0��ʾ������
     * @param [OUT] result {long long*} �ǿ�ʱ�洢�������
     * @return {bool} ��ʾ���������Ƿ�ɹ�, ����ֵ����:
     *    true: �����ɹ�
     *   false: 1) key��value���ʹ���(non-string)
     *          2) key��value���ܽ�����˫���ȸ���ֵ���ַ���
     */
    bool incrbyfloat(const std::string& key, double increment,
        std::string* result = NULL);

    /**
     * @description
     *   �Դ洢��ָ��key����ִֵ�м�1����. ���key������, �Լ�ǰ��key��ֵ��Ϊ0
     * @param [IN] key {const std::string&} ָ���Լ���key
     * @param [OUT] result {long long*} �ǿ�ʱ�洢�Լ����
     * @return {bool} ��ʾ�Լ������Ƿ�ɹ�, ����ֵ����:
     *    true: �����ɹ�
     *   false: 1) key��value���ʹ���(non-string)
     *          2) key��value���ܱ�ʾ�����ֵ��ַ���
     */
    bool decr(const std::string& key, long long* result = NULL);

    /**
     * @description
     *   �Դ洢��ָ��key����ֵ��ȥincrement. ���key������, �Լ�ǰ��key��ֵ��Ϊ0
     * @param [IN] key {const std::string&} ָ���Լ���key
     * @param [IN] increment {long long} ����ֵ, <0��ʾ����
     * @param [OUT] result {long long*} �ǿ�ʱ�洢�Լ����
     * @return {bool} ��ʾ�Լ������Ƿ�ɹ�, ����ֵ����:
     *    true: �����ɹ�
     *   false: 1) key��value���ʹ���(non-string)
     *          2) key��value���ܱ�ʾ�����ֵ��ַ���
     */
    bool decrby(const std::string& key, long long decrement,
        long long* result = NULL);

    /**
     * @description
     *   ���key�Ѿ�����, ����ֵΪ�ַ���, ���value׷�ӵ�ԭ��ֵ�Ľ�β.
     *   ���key������, ��ô�������ȴ���һ�����ַ�����key, ��ִ��׷�Ӳ���
     * @param [IN] key {const std::string&} ָ��׷���ַ�����key
     * @param [IN] value {const std::string&} ׷�ӵ��ַ���ֵ
     * @param [OUT] length {long long*} �ǿ�ʱ�洢׷�Ӻ��ַ����ĳ���
     * @return {bool} ��ʾ�����Ƿ�ɹ�, ����ֵ����:
     *    true: �����ɹ�
     *   false: key��value���ʹ���(non-string)
     */
    bool append(const std::string& key, const std::string& value,
        long long* length = NULL);

    /**
     * @description
     *   ����key������string�ĳ���, ���key������, ����0
     * @param [IN] key {const std::string&} ָ���ַ�����key
     * @param [OUT] length {long long&} �����ַ����ĳ���
     * @return {bool} �����Ƿ�ɹ�, ����ֵ����:
     *    true: �����ɹ�
     *   false: key��value���ʹ���(non-string)
     */
    bool strlen(const std::string& key, long long& length);

public:
    /***************************************************************************
     * list command
     ***************************************************************************/
    /**
     * @description
     *   blpop����������ʽ�б�ĵ���ԭ��. ����lpop�����������汾.
     *   1) �������������б�û���κ�Ԫ�ص�ʱ��, ���ӽ���blpop��������, ֱ�����
     *      �ͻ�������Щ�б��е�����һ���б�pushԪ�ػ��߳�ʱ. ��timeoutΪ0ʱ, ��
     *      ʾ����������
     *   2) ����������б���������һ���ǿ��б�, �򵯳���һ���ǿ��б��ͷ��Ԫ��.
     * @param [IN] vkeys {const std::vector<std::string>&} ��������Ԫ�ص��б���
     * @param [IN] timeout {long long} ��ʱʱ��
     * @param [OUT] key {std::string&} ����Ԫ�ص��б�
     * @param [OUT] value {std::string&} ����Ԫ��
     *   �洢����Ԫ�ص�key�Լ���Ӧ��Ԫ��
     * @return {int} ���ز������, ����ֵ����:
     *    1: �����ɹ�
     *    0: ��ʱ
     *   -1: 1) vkeys��value���ʹ���(non-list)
     *       2) ��������
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int blpop(const std::vector<std::string>& vkeys, long long timeout,
        std::string& key, std::string& value);

    /**
     * @description
     *   brpop����������ʽ�б�ĵ���ԭ��. ����rpop�����������汾. ������Ϊ��
     *   blpop��������, ����brpop�����Ǵ��б�β������Ԫ��.
     * @param [IN] vkeys {const std::vector<std::string>&} ��������Ԫ�ص��б���
     * @param [IN] timeout {long long} ��ʱʱ��
     * @param [OUT] key {std::string&} ����Ԫ�ص��б�
     * @param [OUT] value {std::string&} ����Ԫ��
     *   �洢����Ԫ�ص�key�Լ���Ӧ��Ԫ��
     * @return {int} ���ز������, ����ֵ����:
     *    1: �����ɹ�
     *    0: ��ʱ
     *   -1: 1) keys��value���ʹ���(non-list)
     *       2) ��������
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int brpop(const std::vector<std::string>& vkeys, long long timeout,
        std::string& key, std::string& value);

    /**
     * @description
     *   brpoplpush������rpoplpush�����������汾. ��src����Ԫ��ʱ, ��Ϊ��
     *   rpoplpush����һ��. ��srcΪ��ʱ, redis��������������, ֱ����Ŀͻ�����
     *   src��pushԪ�ػ��߳�ʱ.
     * @param [IN] src {const std::string&} Դ�б����ļ�ֵ
     * @param [IN] dest {const std::string&} Ŀ���б����ļ�ֵ
     * @param [IN] timeout {long long} ��ʱʱ��
     * @param [OUT] result {std::string&} �洢������Ԫ��
     * @return {int} ���ز������, ����ֵ����:
     *    1: �����ɹ�
     *    0: ��ʱ
     *   -1: 1) src��dest��value���ʹ���(non-list)
     *       2) ��������
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int brpoplpush(const std::string& src, const std::string& dest,
        long long timeout, std::string& result);

    /**
     * @description
     *   ����key�������б����±�Ϊindex��Ԫ��. indexΪ����0���±�, ��Ϊ����,
     *   -1��ʾ���һ��Ԫ��, -2��ʾ�����ڶ���Ԫ��.
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] index {int} �±�ֵ
     * @param [OUT] result {std::string&} �洢���
     * @return {int} ���ز������, ����ֵ����:
     *    1: �����±�Ϊindex��Ԫ��
     *    0: �±곬����Χ
     *   -1: �����key��value���ʹ���(non-list)
     */
    int lindex(const std::string& key, int index, std::string& result);

    /**
     * @description
     *   ��key�������б��н�һ����Ԫ��value��ӵ�Ԫ��pivot֮ǰ/��
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] is_before {bool} ָ�����뵽Ԫ��pivot֮ǰ/��
     * @param [IN] pivot {const std::string&} ָ����Ԫ��
     * @param [IN] value {const std::string&} ��Ԫ��
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: ������Ԫ�غ�, ��ǰ�б�Ԫ�ظ���
     *    0: key������, �޲���
     *   -1: 1) key��value���ʹ���(non-list)
     *       2) Ԫ��pivot�����б���
     */
    long long linsert(const std::string& key, bool is_before,
        const std::string& pivot, const std::string& value);

    /**
     * @description
     *   ����key�������б�ĳ���
     * @param [IN] key {const std::string&} �б�����key
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: �б���
     *    0: key������
     *   -1: key��value���ʹ���(non-list)
     */
    long long llen(const std::string& key);

    /**
     * @description
     *   ����������key�������б��еĵ�һ��Ԫ��
     * @param [IN] key {const std::string&} �б�����key
     * @param [OUT] result {std::string&} �洢������Ԫ��
     * @return {int} ���ز������, ����ֵ����:
     *    1: �ɹ������б���Ԫ��
     *    0: key������
     *   -1: key��value���ʹ���(non-list)
     */
    int lpop(const std::string& key, std::string& result);

    /**
     * @description
     *   ��key�������б�ͷ�����β���values�����е�Ԫ��, ������Ԫ��˳���values
     *   �е��෴. ���key������, ����Ԫ��ǰ�ȴ���һ�����б�
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] values {const std::vector<std::string>&} ��Ԫ�ؼ���
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: ������Ԫ�غ�, ��ǰ�б�Ԫ�ظ���
     *   -1: �����key��value���ʹ���(non-list)
     */
    long long lpush(const std::string& key,
        const std::vector<std::string>& values);

    /**
     * @description
     *   ���key���ڲ������б����, ����key�������б�ͷ������Ԫ��value. ���key
     *   ������, �򲻲���Ԫ��
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] value {const std::string&} ��Ԫ��
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: ������Ԫ�غ�, ��ǰ�б�Ԫ�ظ���
     *    0: key������
     *   -1: �����key��value���ʹ���(non-list)
     */
    long long lpushx(const std::string& key, const std::string& value);

    /**
     * @description
     *   ����key�������б���[start, end](������)��Χ�ڵ�Ԫ��. start��endΪ����0
     *   ���±�, ��Ϊ����, -1��ʾ���һ��Ԫ��, -2��ʾ�����ڶ���Ԫ��. ���start��
     *   ���б�ĩβ�±����start > stop, �����Ϊ���б�. ���stop�����б��±�,��
     *   �ᱻ�����б�ĩβ�±�.
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] start {int} ��ʼ�±�
     * @param [IN] end {int} �����±�
     * @param [OUT] result {std::vector<std::string>&} �洢ָ�������Ԫ�ؼ���
     * @return {bool} �����Ƿ�ɹ�, false��ʾ�����key��value���ʹ���(non-list)
     */
    bool lrange(const std::string& key, int start, int stop,
        std::vector<std::string>& result);

    /**
     * @description
     *   �Ƴ�key�������б���ǰcount��ֵ����value��Ԫ��.
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] count {int} �Ƴ�Ԫ�صĸ���. count��ȡֵ����:
     *   count > 0: ���б�ͷ����ʼ�Ƴ�ֵ����valueԪ��
     *   count < 0: ���б�β������ʼ�Ƴ�ֵ����valueԪ��
     *   count = 0: �Ƴ��б������е���valueԪ��
     * @param [IN] value {const std::string&} ��Ҫ�Ƴ���Ԫ��
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: �Ƴ�Ԫ�صĸ���
     *    0: key�����ڻ�value������
     *   -1: �����key��value���ʹ���(non-list)
     * @author chen.tengfei
     * @date 2017-03-29
     */
    long long lrem(const std::string& key, int count, const std::string& value);

    /**
     * @description
     *   ����key�������б����±�index����ֵΪvalue. indexȡֵ��ο�lindex����.
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] index {int} Ԫ���±�
     * @param [IN] value {const std::string&} �µ�Ԫ��ֵ
     * @return {bool} ���ز������, ����ֵ����:
     *    true: ������ֵ�ɹ�
     *   false: 1) key��value���ʹ���(non-list)
     *          2) indexԽ��
     *          3) key������
     * @author chen.tengfei
     * @date 2017-03-29
     */
    bool lset(const std::string& key, int index, const std::string& value);

    /**
     * @description
     *   �޼�key�������б�ʹ��ֻ����[start, stop]ָ����Χ�ڵ�Ԫ��. ���start����
     *   �б�ĩβ�±����start > stop, �����Ϊ���б�. �⽫���key��ɾ��. ���
     *   stop�����б��±�, ��ᱻ�����б�ĩβ�±�
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] start {int} ��ʼ�±�
     * @param [IN] stop {int} �����±�
     * @return {bool} ���ز������, ����ֵ����:
     *    true: �����ɹ�
     *   false: key��value���ʹ���(non-list)
     * @author chen.tengfei
     * @date 2017-03-29
     */
    bool ltrim(const std::string& key, int start, int stop);

    /**
     * @description
     *   ����������key�������б��е����һ��Ԫ��
     * @param [IN] key {const std::string&} �б�����key
     * @param [OUT] result {std::string&} �洢������Ԫ��
     * @return {int} ���ز������, ����ֵ����:
     *    1: �ɹ������б���Ԫ��
     *    0: key������
     *   -1: key��value���ʹ���(non-list)
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int rpop(const std::string& key, std::string& result);

    /**
     * @description
     *   ����������src�б��е����һ��Ԫ��, ���������dest�б��ͷ��. �ò�����ԭ
     *   �Ӳ���. ���src������, �򷵻ؿ��Ҳ����κβ���. ���src��dest��ͬ, ��
     *   ���б�ĩβԪ�ز�����Ԫ���Ƶ��б�ͷ��.
     * @param [IN] src {const std::string&} �б�����src
     * @param [IN] dest {const std::string&} �б�����dest
     * @param [OUT] result {std::string&} �洢������Ԫ��
     * @return {int} ���ز������, ����ֵ����:
     *    1: �����ɹ�
     *    0: src������
     *   -1: 1) src��dest��value���ʹ���(non-list)
     *       2) ��������
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int rpoplpush(const std::string& src, const std::string& dest,
        std::string& result);

    /**
     * @description
     *   ��key�������б�β�����β���values�����е�Ԫ��. ���key������, ����Ԫ��
     *   ǰ�ȴ���һ�����б�
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] values {const std::vector<std::string>&} ��Ԫ�ؼ���
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: ������Ԫ�غ�, ��ǰ�б�Ԫ�ظ���
     *   -1: �����key��value���ʹ���(non-list)
     * @author chen.tengfei
     * @date 2017-03-29
     */
    long long rpush(const std::string& key,
        const std::vector<std::string>& values);

    /**
     * @description
     *   ���key���ڲ������б����, ����key�������б�β������Ԫ��value. ���key
     *   ������, �򲻲���Ԫ��
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] value {const std::string&} ��Ԫ��
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: ������Ԫ�غ�, ��ǰ�б�Ԫ�ظ���
     *   -1: �����key��value���ʹ���(non-list)
     * @author chen.tengfei
     * @date 2017-03-29
     */
    long long rpushx(const std::string& key, const std::string& value);

public:
    /***************************************************************************
     * hash command
     ***************************************************************************/
    /**
     * @description
     *   ��key�����Ĺ�ϣ����ɾ��fields�б�ָ������
     * @param [IN] key {const std::string&} ��ϣ�������key
     * @param [IN] fields {const std::vector<std::string>&} ָ��Ҫɾ������
     * @return {long long} ʵ��ɾ�����������, ������ָ��ɾ����ʵ�ʲ����ڵ���
     *   >0: ɾ�����������
     *   =0: key�����ڻ�ָ�����򲻴���
     *   -1: key��value���ʹ���(non-hash)
     */
    long long hdel(const std::string& key,
        const std::vector<std::string>& fields);

    /**
     * @description
     *   �ж�key�����Ĺ�ϣ����field���Ƿ����
     * @param [IN] key {const std::string&} ��ϣ�������key
     * @param [IN] field {const std::string&} ָ������
     * @return {int} ���ز������, ����ֵ����:
     *    1: �����������
     *    0: key�����ڻ�ָ�����򲻴���
     *   -1: �����key��value���ʹ���(non-hash)
     */
    int hexists(const std::string& key, const std::string& field);

    /**
     * @description
     *   ����key�����Ĺ�ϣ����field�������ֵ
     * @param [IN] key {const std::string&} ��ϣ�������key
     * @param [IN] field {const std::string&} ָ������
     * @param [OUT] result {const std::string&} ָ���������ֵ. ���key�����ڻ�
     *   ָ�����벻����, ���ؿմ�. ��ʱ��Ҫ���ݷ���ֵ�ж�ָ�����Ƿ����, ��Ϊ��
     *   ����ָ����������ַ����ǿմ�
     * @return {int} ���ز������, ����ֵ����:
     *    1: �����������
     *    0: key�����ڻ�ָ�����򲻴���
     *   -1: �����key��value���ʹ���(non-hash)
     */
    int hget(const std::string& key, const std::string& field,
        std::string& result);

    /**
     * @description
     *   ����key�����Ĺ�ϣ�����������������ֵ
     * @param [IN] key {const std::string&} ��ϣ�������key
     * @param [OUT] result {std::map<std::string, std::string>&}
     *   �������Լ�������ֵ��ӳ���. ���key������, ��ӳ���Ϊ��
     * @return {int} ���ز������, ����ֵ����:
     *   >0: ��ϣ������ĸ���
     *    0: key������
     *   -1: �����key��value���ʹ���(non-hash)
     */
    int hgetall(const std::string& key,
        std::map<std::string, std::string>& result);

    /**
     * @description
     *   ʹkey�����Ĺ�ϣ����field������increment. ���key������, �򴴽�һ������
     *   ��key�Ŀչ�ϣ��. ���field������, ����������ǰ�Ƚ�value���ó�0
     * @param [IN] key {const std::string&} ��ϣ�������key
     * @param [IN] field {const std::string&} ָ����������
     * @param [IN] increment {long long} ������, <0��ʾ�Լ�
     * @param [OUT] result {long long*} �ǿ�ʱ�洢�������
     * @return {bool} ��ʾ���������Ƿ�ɹ�, ����ֵ����:
     *    true: �����ɹ�
     *   false: 1) key��value���ʹ���(non-hash)
     *          2) field������ֵ�ǲ��ܱ�ʾ�����ֵ��ַ���
     */
    bool hincrby(const std::string& key, const std::string& field,
        long long increment, long long* result = NULL);

    /**
     * @description
     *   ʹkey�����Ĺ�ϣ����field������increment. ���key������, �򴴽�һ������
     *   ��key�Ŀչ�ϣ��. ���field������, ����������ǰ�Ƚ�value���ó�0
     * @param [IN] key {const std::string&} ��ϣ�������key
     * @param [IN] field {const std::string&} ָ����������
     * @param [IN] increment {double} ����ֵ, <0��ʾ�Լ�
     * @param [OUT] result {std::string*} �ǿ�ʱ�洢�������
     * @return {bool} ��ʾ���������Ƿ�ɹ�, ����ֵ����:
     *    true: �����ɹ�
     *   false: 1) key��value���ʹ���(non-hash)
     *          2) field������ֵ���ܽ�����˫���ȸ���ֵ���ַ���
     */
    bool hincrbyfloat(const std::string& key, const std::string& field,
        double increment, std::string* result = NULL);

    /**
     * @description
     *   ����key�����Ĺ�ϣ��������field�������, ���key�����ڷ��ؿռ�
     * @param [IN] key {const std::string&} ��ϣ�������key
     * @param [OUT] result {std::vector<std::string>&} ��ϣ��������field�������
     * @return {bool} ��ʾ�����Ƿ�ɹ�, ����ֵ����:
     *    true: �����ɹ�
     *   false: key��value���ʹ���(non-hash)
     */
    bool hkeys(const std::string& key, std::vector<std::string>& result);

    /**
     * @description
     *   ����key�����Ĺ�ϣ�����������, ���key�����ڷ���0
     * @param [IN] key {const std::string&} ��ϣ�������key
     * @return {long long} ��ϣ�����������, ����ֵ����:
     *   >0: ��ϣ�����������
     *    0: key������
     *   -1: key��value���ʹ���(non-hash)
     */
    long long hlen(const std::string& key);

    /**
     * @description
     *   ����key�����Ĺ�ϣ����ָ����field���ֵ����.
     * @param [IN] key {const std::string&} ��ϣ�������key
     * @param [IN] fields {const std::vector<std::string>&} ָ�����򼯺�
     * @param [OUT] result {std::vector<std::string*>&} ָ���������ֵ. ���key
     *   �����ڻ�ָ�����򲻴���, ���ؼ��ϵĶ�Ӧλ��Ϊ��ָ��.
     * @return {bool} ��ʾ�����Ƿ�ɹ�, ����ֵ����:
     *    true: �����ɹ�
     *   false: key��value���ʹ���(non-hash)
     */
    bool hmget(const std::string& key, const std::vector<std::string>& fields,
        std::vector<std::string*>& result);

    /**
     * @description
     *   ����key�����Ĺ�ϣ����ָ����field���ֵ. ���ָ����field���Ѵ���, �򸲸�
     *   ԭ���ľ�ֵ. ���key������, ���ȴ���һ���յĹ�ϣ��.
     * @param [IN] key {const std::string&} ��ϣ�������key
     * @param [IN] field_values {const std::map<std::string, std::string>&}
     *   ��������ֵ�Եļ���
     * @return {bool} ��ʾ�����Ƿ�ɹ�, ����ֵ����:
     *    true: �����ɹ�
     *   false: key��value���ʹ���(non-hash)
     */
    bool hmset(const std::string& key,
        const std::map<std::string, std::string>& field_values);

    /**
     * @description
     *   ���ڵ���key�����Ĺ�ϣ���еļ�ֵ��
     * @param [IN] key {const std::string&} ��ϣ�������key
     * @param [IN] cursor {int} �α�ֵ, ��һ�ε���ʹ��0��Ϊ�α�.
     * @param [OUT] result {std::map<std::string, std::string>&} �洢�����, ��
     *   ����׷�ӷ�ʽ�����α��������ӽ��ö�����, Ϊ��ֹ���ܽ���������¸���
     *   �����, �û����ڵ��ñ�����ǰ������ö���.
     * @param [IN] pattern {const char*} glob����ģʽ����, �ǿ�ʱ��Ч
     * @param [IN] count {int} �޶������Ԫ�ص�����, Ĭ��ֵ:10
     * @return {int} ��һ���α�λ��, ����ֵ����:
     *   >0: ��һ���α�λ��
     *    0: ��������
     *   -1: key��value���ʹ���(non-hash)
     */
    int hscan(const std::string& key, int cursor,
        std::map<std::string, std::string>& result,
        const char* pattern = NULL, int count = 10);

    /**
     * @description
     *   ��key�����Ĺ�ϣ����ָ����field���ֵ���ó�value. ���ָ����field���Ѵ�
     *   ��, �򸲸�ԭ���ľ�ֵ. ���key������, ���ȴ���һ���յĹ�ϣ��.
     * @param [IN] key {const std::string&} ��ϣ�������key
     * @param [IN] field {const std::string&} ָ��Ҫ���õ���
     * @param [IN] value {const std::string&} ָ�������Ӧ��ֵ
     * @return {int} ���ز������, ����ֵ����:
     *    1: field�����µ������key������, ��������ֵ
     *    0: field����ڲ�������ֵ
     *   -1: key��value���ʹ���(non-hash)
     */
    int hset(const std::string& key, const std::string& field,
        const std::string& value);

    /**
     * @description
     *   ��key�����Ĺ�ϣ����ָ����field���ֵ���ó�value. ���ָ����field�򲻴�
     *   ��, ������field���ֵΪvalue. ���key������, ���ȴ���һ���յĹ�ϣ��. ��
     *   ��field�Ѵ���, ��ò������κ�Ӱ��.
     * @param [IN] key {const std::string&} ��ϣ�������key
     * @param [IN] field {const std::string&} ָ��Ҫ���õ���
     * @param [IN] value {const std::string&} ָ�������Ӧ��ֵ
     * @return {int} ���ز������, ����ֵ����:
     *    1: field�����µ������key������, ��������ֵ
     *    0: field�������ִ�в���
     *   -1: key��value���ʹ���(non-hash)
     */
    int hsetnx(const std::string& key, const std::string& field,
        const std::string& value);

    /**
     * @description
     *   ����key�����Ĺ�ϣ����ָ����field���ֵ���ַ�������. ���ָ����field��
     *   ���ڻ���key������, �򷵻�0.
     * @param [IN] key {const std::string&} ��ϣ�������key
     * @param [IN] field {const std::string&} ָ��Ҫ���õ���
     * @return {int} ���ز������, ����ֵ����:
     *   >0: field���ֵ���ַ�������
     *    0: field�򲻴��ڻ���key������
     *   -1: key��value���ʹ���(non-hash)
     */
    long long hstrlen(const std::string& key, const std::string& field);

    /**
     * @description
     *   ����key�����Ĺ�ϣ��������ֵ�ļ���, ���key�����ڷ��ؿ��б�
     * @param [IN] key {const std::string&} ��ϣ�������key
     * @param [OUT] result {std::vector<std::string>&} ��ϣ�������е�ֵ
     * @return {bool} ��ʾ�����Ƿ�ɹ�, ����ֵ����:
     *    true: �����ɹ�
     *   false: key��value���ʹ���(non-hash)
     */
    bool hvals(const std::string& key, std::vector<std::string>& result);

public:
    /***************************************************************************
     * set command
     ***************************************************************************/
    /**
     * @description
     *   ��key�����ļ��������ָ���ĳ�Ա. ���ָ���ĳ�Ա�Ѿ�����, �����. ���
     *   key������, �ȴ���һ���µļ���.
     * @param [IN] key {const std::string&} ���϶����key
     * @param [IN] member {const std::vector<std::string>&} ָ����ӵĳ�Ա�б�
     * @return {long long} ���ز������, ����ֵ����:
     *   >=0: �����ӵ�Ԫ�ظ���, �������Ѿ����ڵļ���Ԫ��
     *    -1: �����key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sadd(const std::string& key,
        const std::vector<std::string>& member);

    /**
     * @description
     *   ����key�����ļ��ϵĻ���(���ϵ�Ԫ�ظ���)
     * @param [IN] key {const std::string&} ���϶����key
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: ���ϵĻ���
     *    0: key������
     *   -1: �����key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long scard(const std::string& key);

    /**
     * @description
     *   ����ָ����һ�鼯���е�һ�����������༯�ϵĲ. �����ڵ�key�������ռ�.
     * @param [IN] vkeys {const std::vector<std::string>&} ָ����һ�鼯��
     * @param [OUT] result {std::vector<std::string>&} �洢���صĲ
     * @return {bool} ���ز������, false��ʾ�����keys��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    bool sdiff(const std::vector<std::string>& vkeys,
        std::vector<std::string>& result);

    /**
     * @description
     *   �������и������ϵĽ���. �����ڵ�key�������ռ�.
     * @param [IN] vkeys {const std::vector<std::string>&} ָ����һ�鼯��
     * @param [OUT] result {std::vector<std::string>&} �洢���صĽ���
     * @return {bool} ���ز������, false��ʾ�����keys��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    bool sinter(const std::vector<std::string>& vkeys,
        std::vector<std::string>& result);

    /**
     * @description
     *   �������и������ϵĲ���. �����ڵ�key�������ռ�.
     * @param [IN] vkeys {const std::vector<std::string>&} ָ����һ�鼯��
     * @param [OUT] result {std::vector<std::string>&} �洢���صĲ���
     * @return {bool} ���ز������, false��ʾ�����keys��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    bool sunion(const std::vector<std::string>& vkeys,
        std::vector<std::string>& result);

    /**
     * @description
     *   ����ָ����һ�鼯���е�һ�����������༯�ϵĲ, ���ѽ���洢�ڼ���dest
     *   ��. ���dest�����Ѵ���, �򸲸�ԭ��Ԫ��. �����ڵ�key�������ռ�.
     * @param [IN] dest {const std::string&} ��ż������ļ��϶����key
     * @param [IN] vkeys {const std::vector<std::string>&} ָ����һ�鼯��
     * @return {long long} ���ؽ������Ԫ�ظ���, ����ֵ����:
     *   >=0: �������Ԫ�ظ���
     *    -1: ����򼯺϶�������ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sdiffstore(const std::string& dest,
        const std::vector<std::string>& vkeys);

    /**
     * @description
     *   �������и������ϵĽ���, ���ѽ���洢�ڼ���dest��. ���dest�����Ѵ���,
     *   �򸲸�ԭ��Ԫ��. �����ڵ�key�������ռ�.
     * @param [IN] dest {const std::string&} ��ż������ļ��϶����key
     * @param [IN] vkeys {const std::vector<std::string>&} ָ����һ�鼯��
     * @return {long long} ���ؽ������Ԫ�ظ���, ����ֵ����:
     *   >=0: �������Ԫ�ظ���
     *    -1: ����򼯺϶�������ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sinterstore(const std::string& dest,
        const std::vector<std::string>& vkeys);

    /**
     * @description
     *   �������и������ϵĲ���, ���ѽ���洢�ڼ���dest��. ���dest�����Ѵ���,
     *   �򸲸�ԭ��Ԫ��. �����ڵ�key�������ռ�.
     * @param [IN] dest {const std::string&} ��ż������ļ��϶����key
     * @param [IN] vkeys {const std::vector<std::string>&} ָ����һ�鼯��
     * @return {long long} ���ؽ������Ԫ�ظ���, ����ֵ����:
     *   >=0: �������Ԫ�ظ���
     *    -1: ����򼯺϶�������ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sunionstore(const std::string& dest,
        const std::vector<std::string>& vkeys);

    /**
     * @description
     *   �жϸ�����Ԫ��member�Ƿ���key�������ϵĳ�Ա
     * @param [IN] key {const std::string&} ���϶����key
     * @param [IN] member {const std::string&} ������Ԫ��
     * @return {int} �����жϽ��, ����ֵ����:
     *    1: ������Ԫ���Ǽ��ϳ�Ա
     *    0: ������Ԫ�ز��Ǽ��ϳ�Ա����key������
     *   -1: ����򼯺϶�������ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int sismember(const std::string& key, const std::string& member);

    /**
     * @description
     *   ��ȡkey�����ļ��ϵ����г�Ա. Ч����ֻ��һ��keyֵ��sinter����һ��.
     * @param [IN] key {const std::string&} ���϶����key
     * @param [OUT] result {std::vector<std::string>&} ��ŷ��ؽ��
     * @return {int} ���ز������, ����ֵ����:
     *   >0: ����Ԫ�ظ���
     *    0: key������
     *   -1: �����keys��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int smembers(const std::string& key, std::vector<std::string>& result);

    /**
     * @description
     *   �Ѹ�����Ԫ��member��src�����ļ����Ƶ�dest�����ļ���. �ò�����һ��ԭ�Ӳ�
     *   ��. �ڱ�Ŀͻ��˿���, �ڸ���ʱ��Ԫ��memberֻ������src�л�dest��.
     *   1) src���ϲ����ڻ��߲�����Ԫ��member, �����κβ���������0.
     *   2) ��Ԫ��member�Ѿ�������dest������, ��ֻ��Ԫ��member��src�������Ƴ�.
     * @param [IN] src {const std::string&} src���϶���ļ�ֵ
     * @param [IN] dest {const std::string&} dest���϶���ļ�ֵ
     * @param [IN] member {const std::string&} ������Ҫ�ƶ���Ԫ��
     * @return {int} �����жϽ��, ����ֵ����:
     *    1: ������Ԫ�ر��ƶ�
     *    0: ������Ԫ�ز���src���ϳ�Ա����src������
     *   -1: �������src/dest���϶�������ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int smove(const std::string& src, const std::string& dest,
        const std::string& member);

    /**
     * @description
     *   ����Ĵ�key�����ļ����е���һ��Ԫ��, �����ص�����Ԫ��.
     * @param [IN] key {const std::string&} ���϶����key
     * @param [OUT] result {std::string&} ��ŵ�����Ԫ��
     * @return {int} �����жϽ��, ����ֵ����:
     *    1: �������һ��Ԫ��
     *    0: key������
     *   -1: �����key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int spop(const std::string& key, std::string& result);

    /**
     * @description
     *   ����Ĵ�key�����ļ����е���count��Ԫ��, �����ص�����Ԫ��.
     *   3.2���ϰ汾֧��.
     * @param [IN] key {const std::string&} ���϶����key
     * @param [OUT] result {std::vector<std::string>&} ��ŵ�����Ԫ��
     * @param [IN] count {int} ����Ԫ�صĸ���, ȡֵ����:
     *   >0: ����count����Ԫ��, ���count���ڼ���Ԫ������, �򵯳�����Ԫ��
     *    0: ������, ���ؿ��б�
     * @return {int} �����жϽ��, ����ֵ����:
     *   >0: ����Ԫ�صĸ���
     *    0: key������
     *   -1: countΪ��ֵ��key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int spop(const std::string& key,
        std::vector<std::string>& result, int count);

    /**
     * @description
     *   ����Ĵ�key�����ļ����з���һ��Ԫ��.
     * @param [IN] key {const std::string&} ���϶����key
     * @param [OUT] result {std::string&} ��ŷ��ص�Ԫ��
     * @return {int} �����жϽ��, ����ֵ����:
     *    1: �������һ��Ԫ��
     *    0: key������
     *   -1: �����key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int srandmember(const std::string& key, std::string& result);

    /**
     * @description
     *   ����Ĵ�key�����ļ����з���count��Ԫ��.
     * @param [IN] key {const std::string&} ���϶����key
     * @param [OUT] result {std::string&} ��ŷ��ص�Ԫ��
     * @param [IN] count {int} ����Ԫ�صĸ���, ȡֵ����:
     *   >0: ����count�����ظ���Ԫ��, ���count���ڼ���Ԫ������, �򷵻�����Ԫ��
     *    0: ���ؿ��б�
     *   <0: ����count��Ԫ��(���ظ�)
     * @return {int} �����жϽ��, ����ֵ����:
     *   >0: ����Ԫ�صĸ���
     *    0: key�����ڻ�count=0
     *   -1: �����key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int srandmember(const std::string& key,
        std::vector<std::string>& result, int count);

    /**
     * @description
     *   ��key�����ļ������Ƴ�ָ��Ԫ��. ��������в�����ָ��Ԫ��, �����. ���
     *   key������, �򵱳ɿռ���, ������0.
     * @param [IN] key {const std::string&} ���϶����key
     * @param [IN] member {const std::vector<std::string>&} ��Ҫ�Ƴ���Ԫ���б�
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: �Ƴ���Ԫ�ظ���, �����������Ͳ����ڵ�Ԫ��
     *    0: key�����ڻ���û��Ԫ�ر��Ƴ�
     *   -1: �����key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    long long srem(const std::string& key,
        const std::vector<std::string>& member);

    /**
     * @description
     *   ���ڵ���key�����ļ����е�Ԫ��
     * @param [IN] key {const std::string&} ���϶����key
     * @param [IN] cursor {int} �α�ֵ, ��һ�ε���ʹ��0��Ϊ�α�.
     * @param [OUT] result {std::vector<std::string>&} �洢�����, �ڲ���׷�ӷ�
     *   ʽ�����α��������ӽ��ö�����, Ϊ��ֹ���ܽ���������¸��������, ��
     *   �����ڵ��ñ�����ǰ������ö���.
     * @param [IN] pattern {const char*} glob����ģʽ����, �ǿ�ʱ��Ч
     * @param [IN] count {int} �޶������Ԫ�ص�����, Ĭ��ֵ:10
     * @return {int} ��һ���α�λ��, ����ֵ����:
     *   >0: ��һ���α�λ��
     *    0: ��������
     *   -1: key��value���ʹ���(non-hash)
     */
    int sscan(const std::string& key, int cursor,
        std::vector<std::string>& result,
        const char* pattern = NULL, int count = 10);

public:
    /***************************************************************************
     * zset command
     ***************************************************************************/
    /**
     * @description
     *   ��key���������򼯺����������ָ����Ԫ��. ÿ��Ԫ�ض�Ӧһ������.
     *   1) ��������ĳ�Ա�Ѿ����������򼯺���, ��������������������λ��
     *   2) ���key������, ���½�һ�����򼯺ϲ�������/��Ա��ӵ����򼯺���
     *   3) ���key��value���ʹ���(non-sortedset), �򷵻�һ������
     *   4) ����֧��˫����64λ������
     *   5) ���򼯺ϰ��շ�����������, �������Ԫ�ط���, ����Ԫ�ص��ֵ�˳������
     * @param [IN] key {const std::string&} ���򼯺϶����key
     * @param [IN] member_score
     *   {const std::vector<std::pair<std::string, double>>&} Ԫ��/������
     * @param [IN] nx_or_xx {int} �Ƿ�ʹ��NX��XX��־, Ĭ��ֵΪ0, ȡֵ��Χ����:
     *    1: ʹ��XX��־, ֻ�����Ѿ����������򼯺��е�Ԫ�صķ���, ��������Ԫ��
     *    0: ����NX/XX��־
     *   -1: ʹ��NX��־, �������Ѿ����������򼯺��е�Ԫ�صķ���, ֻ������Ԫ��
     * @param [IN] is_change {bool} �Ƿ��޸ķ���ֵ, Ĭ��ֵΪfalse, ȡֵ����:
     *    true: ����ֵΪ���б��ı��Ԫ�صĸ���, �����¼�Ԫ�غͷ������µ�Ԫ��
     *   false: ����ֵΪ�����ӵ�Ԫ�ظ���
     * @return {int} ���ز������, ����ֵ����:
     *   >=0: �����ӵ�Ԫ�ظ���
     *   -1: �����key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    long long zadd(const std::string& key,
        const std::vector< std::pair<std::string, double> >& member_score,
        int nx_or_xx = 0, bool is_change = false);

    long long zadd_nx(const std::string& key,
        const std::vector< std::pair<std::string, double> >& member_score,
        bool is_change = false);

    long long zadd_xx(const std::string& key,
        const std::vector< std::pair<std::string, double> >& member_score,
        bool is_change = false);

    long long zadd_ch(const std::string& key,
        const std::vector< std::pair<std::string, double> >& member_score,
        int nx_or_xx = 0);

    /**
     * @description
     *   ��key���������򼯺���ָ����Ԫ��member�ķ���ֵ�����ۼ�. ��Ϊ��zincrby��
     *   ��һ��.
     * @param [IN] key {const std::string&} ���򼯺϶����key
     * @param [IN] score {double} �ۼӵķ���
     * @param [IN] member {const std::string&} ��Ҫ�ۼӵĳ�Ա
     * @param [OUT] result {std::string&} �洢�ۼӽ��
     * @return {bool} ���ز������, ����ֵ����:
     *    true: �ۼӳɹ�
     *   false: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    bool zadd_incr(const std::string& key, double score,
        const std::string& member, std::string& result);

    /**
     * @description
     *   ����key���������򼯺ϵĻ���(���ϵ�Ԫ�ظ���).
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: ���ϵĻ���.
     *    0: key������.
     *   -1: �����key��value���ʹ���(non-set).
     * @author chen.tengfei
     * @date 2017-04-05
     */
    long long zcard(const std::string& key);

    /**
     * @description
     *   ����key���������򼯺���, ����ֵ��[min, max](������)��Χ�ڵ�Ԫ�ظ���.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {double} ������������.
     * @pzrzm [IN] max {double} ������������.
     * @return {long long} ����ָ��������Χ��Ԫ�ظ���, ����ֵ����:
     *   >=0: ָ��������Χ��Ԫ�ظ���
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-05
     */
    long long zcount(const std::string& key, double min, double max);

    /**
     * @description
     *   ����key���������򼯺���, ����ֵ��[min, max](������)��Χ�ڵ�Ԫ�ظ���.
     *   1) min��max����ʹ��-inf��+inf. ���������ڲ�֪�����򼯺���ͺ���߷���ֵ
     *      �������ʹ�ñ�����.
     *   2) Ĭ�������, ���������ȡֵʹ�ñ�����, �����ڷ���ֵǰ�����'('������
     *      ʹ�ÿ�����.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {const std::string&} ������������.
     * @pzrzm [IN] max {const std::string&} ������������.
     * @return {long long} ����ָ��������Χ��Ԫ�ظ���, ����ֵ����:
     *   >=0: ָ��������Χ��Ԫ�ظ���
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @param [IN/OUT] name {type}
     * @return {type}
     * @author chen.tengfei
     * @date 2017-04-15
     */
    long long zcount(const std::string& key, const std::string& min,
        const std::string& max);

    /**
     * @description
     *   ��key���������򼯺���ָ����Ԫ��member�ķ���ֵ�ۼ�increment.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] increment {double} �ۼ�ֵ
     * @param [IN] member {const std::string&} ��Ҫ�ۼӵĳ�Ա
     * @param [OUT] result {std::string*} ��Ϊ��ʱ, �洢�ۼӽ��
     * @return {int} ���ز������, ����ֵ����:
     *    true: �ۼӳɹ�
     *   false: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-05
     */
    bool zincrby(const std::string& key, double increment,
        const std::string& member, std::string* result = NULL);

    /**
     * @description
     *   �������numkeys�����򼯺ϵĽ���, ��������������dest��.
     * @param [IN] dest {const std::string&} ��ż����������򼯺϶���
     * @param [IN] vkeys {const std::vector<std::string>&}
     *   ������һ�����򼯺�/����(���м��ϳ�Ա�ķ�ֵ������Ϊ1).
     * @param [IN] weights {const std::vector<double>*} ���򼯺϶�Ӧ��Ȩ���б�.
     *   ÿһ�����϶�Ӧһ��Ȩ��ֵ. ��Ȩ���б�Ϊ��ʱ, ��ʾĬ��Ȩ��ֵΪ1.
     * @param [IN] aggregate {const char*} ָ��������ľۺϷ�ʽ. Ĭ��ֵΪ"SUM",
     *   ȡֵ����:
     *   "SUM": �����м�����ĳ����Ա��scoreֵ֮����Ϊ������иó�Ա��scoreֵ
     *   "MIN": �����м�����ĳ����Ա��scoreֵ����Сֵ��Ϊ������иó�Ա��scoreֵ
     *   "MAX": �����м�����ĳ����Ա��scoreֵ�����ֵ��Ϊ������иó�Ա��scoreֵ
     * @return {long long} �����dest��Ԫ�صĸ���, ȡֵ����:
     *   >=0: �����dest��Ԫ�صĸ���
     *    -1: �����dest/keys��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-06
     */
    long long zinterstore(const std::string& dest,
        const std::vector<std::string>& vkeys,
        const std::vector<double>* weights = NULL,
        const char* aggregate = "SUM");

    /**
     * @description
     *   �������numkeys�����򼯺ϵĽ���, ��������������dest��.
     * @author chen.tengfei
     * @date 2017-04-06
     */
    long long zunionstore(const std::string& dest,
        const std::vector<std::string>& vkeys,
        const std::vector<double>* weights = NULL,
        const char* aggregate = "SUM");

    /**
     * @description
     *   �����򼯺ϵ����г�Ա������ͬ����ֵʱ, ���򼯺ϵ�Ԫ�ػ���ݳ�Ա���ֵ���
     *   (lexicographical ordering)����������, �������������Է���key����������
     *   ������, ֵ����Ԫ��min��Ԫ��max֮��ĳ�Ա����. min��max���÷��ο�
     *   zrangebylex����.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {const std::string&} ����Ԫ����Сֵ
     * @param [IN] max {const std::string&} ����Ԫ�����ֵ
     * @return {long long} ����ָ�������ڵĳ�Ա����, ����ֵ����:
     *   >=0: ָ�������ڵĳ�Ա����
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    long long zlexcount(const std::string& key,
        const std::string& min, const std::string& max);

    /**
     * @description
     *   ����key���������򼯺���[start, stop](������)ָ����Χ�ڵ�Ԫ��. ���г�
     *   Ա���շ���ֵ��������, ������ͬ��Ԫ�ذ����ֵ�����������. start��stop�ǻ�
     *   ��0���±�, ��Ϊ����, -1��ʾ���һ��Ԫ��, -2��ʾ�����ڶ���Ԫ��. ���
     *   start�����б�ĩβ�±����start > stop, ���ؽ����Ϊ���б�. ���stop����
     *   �б��±�, ��ᱻ�����б�ĩβ�±�.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] start {int} ��ʼ�±�
     * @param [IN] stop {int} �����±�
     * @param [OUT] result {std::vector<std::string>&} �洢���صĽ����
     * @return {int} ���ؽ������С, ����ֵ����:
     *   >=0: �������С
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrange(const std::string& key, int start, int stop,
        std::vector<std::string>& result);

    /**
     * @description
     *   ����key���������򼯺���[start, stop](������)ָ����Χ�ڵ�Ԫ�ؼ������.
     *   start/stopȡֵ��Χ�ο�zrange����.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] start {int} ��ʼ�±�
     * @param [IN] stop {int} �����±�
     * @param [OUT] result {std::vector<std::pair<std::string, double>>&}
     *   �洢���صĽ����
     * @return {int} ���ؽ������С, ����ֵ����:
     *   >=0: �������С
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrange_with_scores(const std::string& key, int start, int stop,
        std::vector< std::pair<std::string, double> >& result);

    /**
     * @description
     *   �����򼯺ϵ����г�Ա������ͬ����ֵʱ, ���򼯺ϵ�Ԫ�ػ���ݳ�Ա���ֵ���
     *   (lexicographical ordering)����������, �������������Է���key����������
     *   ������, ֵ����Ԫ��min��Ԫ��max֮��ĳ�Ա.
     *   1) ������򼯺���Ԫ�صķ�����һ��, �򷵻ؽ��Ԫ�ؽ���׼ȷ.
     *   2) �ַ������ն������ֽ�������бȽ�, ��С��������. ����ַ�����ʹ���˷�
     *      ASCII�ַ���(����UTF-8), ���Ӱ�췵�ؽ��
     *   3) ��Ч��min��max������'('��'['��ͷ, ��ʾ�����򲻰���min/maxԪ��.
     *   4) min��maxλ�ÿ���ʹ��'-'��'+'��ʾ��Сֵ�����ֵ.
     *      e.g. zrangebylex("foo", "-", "+", out)
     *           ���᷵�����򼯺�foo�����е�Ԫ��
     *   5) max��min��λ�ò��ܽ���, ���򽫻᷵�ؿռ�
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {const std::string&} ����Ԫ����Сֵ
     * @param [IN] max {const std::string&} ����Ԫ�����ֵ
     * @param [OUT] result {std::vector<std::string>&} �洢�����
     * @param [IN] offset {int} �ӽ������ѡȡ���±���ʼֵ, ȡֵ��Χ:
     *   >=0: ������е��±�ֵ
     *    -1: ���������Ƭ, Ĭ��ֵ
     * @paran [IN] count {int} �ӽ����offsetָ����λ�ÿ�ʼѡȡ��Ԫ������
     *   >0: Ϊ��Чֵ
     *    0: Ĭ��ֵ, ��offset < 0ʱ, ����offset��count��������
     * @return {int} ���ؽ������С, ����ֵ����:
     *   >=0: �������С
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrangebylex(const std::string& key, const std::string& min,
        const std::string& max, std::vector<std::string>& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   ����key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ��
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {double} ������������.
     * @pzrzm [IN] max {double} ������������.
     * @param [OUT] result {std::vector<std::string>&} �洢�����
     * @param [IN] offset {int} �ӽ������ѡȡ���±���ʼֵ, ȡֵ��Χ:
     *   >=0: ������е��±�ֵ
     *    -1: ���������Ƭ, Ĭ��ֵ
     * @paran [IN] count {int} �ӽ����offsetָ����λ�ÿ�ʼѡȡ��Ԫ������
     *   >0: Ϊ��Чֵ
     *    0: Ĭ��ֵ, ��offset < 0ʱ, ����offset��count��������
     * @return {int} ���ؽ������С, ����ֵ����:
     *   >=0: �������С
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrangebyscore(const std::string& key, double min, double max,
        std::vector<std::string>& result, int offset = -1, int count = 0);

    /**
     * @description
     *   ����key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ��
     *   1) min��max����ʹ��-inf��+inf. ���������ڲ�֪�����򼯺���ͺ���߷���ֵ
     *      �������ʹ�ñ�����.
     *   2) Ĭ�������, ���������ȡֵʹ�ñ�����, �����ڷ���ֵǰ�����'('������
     *      ʹ�ÿ�����.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {const std::string&} ������������.
     * @pzrzm [IN] max {const std::string&} ������������.
     * @param [OUT] result {std::vector<std::string>&} �洢�����
     * @param [IN] offset {int} �ӽ������ѡȡ���±���ʼֵ(����0), ȡֵ��Χ:
     *   >=0: ������е��±�ֵ
     *    -1: ���������Ƭ, Ĭ��ֵ
     * @paran [IN] count {int} �ӽ����offsetָ����λ�ÿ�ʼѡȡ��Ԫ������
     *   >0: Ϊ��Чֵ
     *    0: Ĭ��ֵ, ��offset < 0ʱ, ����offset��count��������
     * @return {int} ���ؽ������С, ����ֵ����:
     *   >=0: �������С
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrangebyscore(const std::string& key, const std::string& min,
        const std::string& max, std::vector<std::string>& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   ����key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ�ؼ���
     *   ����
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {double} ������������.
     * @pzrzm [IN] max {double} ������������.
     * @param [OUT] result {std::vector<std::pair<std::string, double>>&}
     *   �洢�����
     * @param [IN] offset {int} �ӽ������ѡȡ���±���ʼֵ, ȡֵ��Χ:
     *   >=0: ������е��±�ֵ
     *    -1: ���������Ƭ, Ĭ��ֵ
     * @paran [IN] count {int} �ӽ����offsetָ����λ�ÿ�ʼѡȡ��Ԫ������
     *   >0: Ϊ��Чֵ
     *    0: Ĭ��ֵ, ��offset < 0ʱ, ����offset��count��������
     * @return {int} ���ؽ������С, ����ֵ����:
     *   >=0: �������С
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrangebyscore_with_scores(const std::string& key,
        double min, double max,
        std::vector< std::pair<std::string, double> >& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   ����key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ�ؼ���
     *   ����
     *   1) min��max����ʹ��-inf��+inf. ���������ڲ�֪�����򼯺���ͺ���߷���ֵ
     *      �������ʹ�ñ�����.
     *   2) Ĭ�������, ���������ȡֵʹ�ñ�����, �����ڷ���ֵǰ�����'('������
     *      ʹ�ÿ�����.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {const std::string&} ������������.
     * @pzrzm [IN] max {const std::string&} ������������.
     * @param [OUT] result {std::vector<std::pair<std::string, double>>&}
     *   �洢�����
     * @param [IN] offset {int} �ӽ������ѡȡ���±���ʼֵ, ȡֵ��Χ:
     *   >=0: ������е��±�ֵ
     *    -1: ���������Ƭ, Ĭ��ֵ
     * @paran [IN] count {int} �ӽ����offsetָ����λ�ÿ�ʼѡȡ��Ԫ������
     *   >0: Ϊ��Чֵ
     *    0: Ĭ��ֵ, ��offset < 0ʱ, ����offset��count��������
     * @return {int} ���ؽ������С, ����ֵ����:
     *   >=0: �������С
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrangebyscore_with_scores(const std::string& key,
        const std::string& min, const std::string& max,
        std::vector< std::pair<std::string, double> >& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   ����Ԫ��member��key���������򼯺��е�����, ���򼯺ϰ��շ���ֵ��������.
     *   ������(���±�)�ǻ���0��, Ҳ����˵, scoreֵ��С�ĳ�Ա����Ϊ0.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] member {const std::string&} ָ����Ԫ��member.
     * @return {int} ����ָ��Ԫ�������򼯺��е�����, ����ֵ����:
     *   >=0: ָ��Ԫ�������򼯺��е�����
     *    -1: 1) �����key��value���ʹ���(non-sortedset)
     *        2) key�����ڻ���member������
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrank(const std::string& key, const std::string& member);

    /**
     * @description
     *   ��key���������򼯺����Ƴ�ָ��Ԫ��. ��������в�����ָ��Ԫ��, �����. ��
     *   ��key������, �򵱳ɿռ���, ������0.
     * @param [IN] key {const std::string&} ���϶����key
     * @param [IN] member {const std::vector<std::string>&} ��Ҫ�Ƴ���Ԫ���б�
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: �Ƴ���Ԫ�ظ���, �����������Ͳ����ڵ�Ԫ��
     *    0: key�����ڻ���û��Ԫ�ر��Ƴ�
     *   -1: �����key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    long long zrem(const std::string& key,
        const std::vector<std::string>& member);

    /**
     * @description
     *   �����򼯺ϵ����г�Ա������ͬ����ֵʱ, ���򼯺ϵ�Ԫ�ػ���ݳ�Ա���ֵ���
     *   (lexicographical ordering)����������, ���������������Ƴ�key����������
     *   ������, ֵ����Ԫ��min��Ԫ��max֮��ĳ�Ա.
     *   min��max�ĺ�����ο�zrangebylex����.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {const std::string&} ����Ԫ����Сֵ
     * @param [IN] max {const std::string&} ����Ԫ�����ֵ
     * @return {int} �����Ƴ�Ԫ�ص�����, ����ֵ����:
     *   >=0: �Ƴ�Ԫ�ص�����
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zremrangebylex(const std::string& key, const std::string& min,
        const std::string& max);

    /**
     * @description
     *   �Ƴ�key���������򼯺���[start, stop](������)ָ����Χ�ڵ�Ԫ��.
     *   start��stop�ĺ�����ο�zrange����.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] start {int} ��ʼ�±�
     * @param [IN] stop {int} �����±�
     * @return {int} �����Ƴ�Ԫ�ص�����, ����ֵ����:
     *   >=0: �Ƴ�Ԫ�ص�����
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zremrangebyrank(const std::string& key, int start, int stop);

    /**
     * @description
     *   �Ƴ�key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ��
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {double} ������������.
     * @pzrzm [IN] max {double} ������������.
     * @return {int} �����Ƴ�Ԫ�ص�����, ����ֵ����:
     *   >=0: �Ƴ�Ԫ�ص�����
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zremrangebyscore(const std::string& key, double min, double max);

    /**
     * @description
     *   �Ƴ�key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ��
     *   1) min��max����ʹ��-inf��+inf. ���������ڲ�֪�����򼯺���ͺ���߷���ֵ
     *      �������ʹ�ñ�����.
     *   2) Ĭ�������, ���������ȡֵʹ�ñ�����, �����ڷ���ֵǰ�����'('������
     *      ʹ�ÿ�����.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {const std::string&} ������������.
     * @pzrzm [IN] max {const std::string&} ������������.
     * @return {int} �����Ƴ�Ԫ�ص�����, ����ֵ����:
     *   >=0: �Ƴ�Ԫ�ص�����
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zremrangebyscore(const std::string& key, const std::string& min,
        const std::string& max);

    /**
     * @description
     *   ����key���������򼯺���[start, stop](������)ָ����Χ�ڵ�Ԫ��. ��������
     *   ֮��, �����ͷ���ֵͬzrange����һ��.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrange(const std::string& key, int start, int stop,
        std::vector<std::string>& result);

    /**
     * @description
     *   ����key���������򼯺���[start, stop](������)ָ����Χ�ڵ�Ԫ�ؼ������.
     *   ��������֮��, �����ͷ���ֵͬzrange_with_scores����һ��.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrange_with_scores(const std::string& key,
        int start, int stop,
        std::vector< std::pair<std::string, double> >& result);

    /**
     * @description
     *   �����򼯺ϵ����г�Ա������ͬ����ֵʱ, ���򼯺ϵ�Ԫ�ػ���ݳ�Ա���ֵ���
     *   (lexicographical ordering)����������, �������������Է���key����������
     *   ������, ֵ����Ԫ��min��Ԫ��max֮��ĳ�Ա.
     *   ��������֮��, �����ͷ���ֵͬzrangebylex����һ��.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrangebylex(const std::string& key, const std::string& min,
        const std::string& max, std::vector<std::string>& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   ����key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ��.
     *   ��������֮��, �����ͷ���ֵͬzrangebyscore����һ��.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrangebyscore(const std::string& key, double min, double max,
        std::vector<std::string>& result, int offset = -1, int count = 0);

    /**
     * @description
     *   ����key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ��.
     *   ��������֮��, �����ͷ���ֵͬzrangebyscore����һ��.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrangebyscore(const std::string& key, const std::string& min,
        const std::string& max, std::vector<std::string>& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   ����key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ�ؼ����
     *   ��. ��������֮��, �����ͷ���ֵͬzrangebyscore_with_scores����һ��.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrangebyscore_with_scores(const std::string& key,
        double min, double max,
        std::vector< std::pair<std::string, double> >& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   ����key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ�ؼ����
     *   ��. ��������֮��, �����ͷ���ֵͬzrangebyscore_with_scores����һ��.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrangebyscore_with_scores(const std::string& key,
        const std::string& min, const std::string& max,
        std::vector< std::pair<std::string, double> >& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   ����Ԫ��member��key���������򼯺��е�����, ���򼯺ϰ��շ���ֵ�ݼ�����.
     *   ������(���±�)�ǻ���0��, Ҳ����˵, scoreֵ���ĳ�Ա����Ϊ0.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] member {const std::string&} ָ����Ԫ��member.
     * @return {int} ����ָ��Ԫ�������򼯺��е�����, ����ֵ����:
     *   >=0: ָ��Ԫ�������򼯺��е�����
     *    -1: 1) �����key��value���ʹ���(non-sortedset)
     *        2) key�����ڻ���member������
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrank(const std::string& key, const std::string& member);

    /**
     * @description
     *   ���ڵ���key���������򼯺��еļ�ֵ��
     * @param [IN] key {const std::string&} ���򼯺϶����key
     * @param [IN] cursor {int} �α�ֵ, ��һ�ε���ʹ��0��Ϊ�α�.
     * @param [OUT] result {std::vector< std::pair<std::string, double> >&}
     *   �洢�����, �ڲ���׷�ӷ�ʽ�����α��������ӽ��ö�����, Ϊ��ֹ���ܽ��
     *   �������¸��������,�û����ڵ��ñ�����ǰ������ö���.
     * @param [IN] pattern {const char*} glob����ģʽ����, �ǿ�ʱ��Ч
     * @param [IN] count {int} �޶������Ԫ�ص�����, Ĭ��ֵ:10
     * @return {int} ��һ���α�λ��, ����ֵ����:
     *   >0: ��һ���α�λ��
     *    0: ��������
     *   -1: key��value���ʹ���(non-hash)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zscan(const std::string& key, int cursor,
        std::vector< std::pair<std::string, double> >& result,
        const char* pattern = NULL, int count = 10);

    /**
     * @description
     *   ����key���������򼯺���Ԫ��member�ķ���.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] member {const std::string&} ָ����Ԫ��member.
     * @param [OUT] soube {std::string&} �洢ָ��Ԫ�صķ���
     * @return {int} ���ز������, ����ֵ����:
     *    1: �ɹ�����ָ��Ԫ�صķ���
     *    0: key�����ڻ���member������
     *   -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zscore(const std::string& key, const std::string& member,
        std::string& score);

public:
    /***************************************************************************
     * keys command
     ***************************************************************************/

    /**
     * ɾ��ָ����һ����һ��keys, ���Բ����ڵ�key
     * @param [IN] vkeys {const std::vector<std::string>&} ָ����һ��keys
     * @return {int} ����ʵ��ɾ����key�ĸ���, ����:
     *   0: δɾ���κ�key
     *  >0: ʵ��ɾ����key�ĸ���, ��ֵ����С������key�ĸ���
     *  -1: ����
     */
    int del(const std::vector<std::string>& vkeys);
    int del(const std::string& key);

    /**
     * @description
     *   ���л�����key��ֵ, ���������л����ֵ. ʹ��RESTORE����ɽ���ֵ�����л�
     *   ����redis key��.
     * @param [IN] key {const std::string&} ������key
     * @param [OUT] result {std::string&} �洢���л����ֵ
     * @return {int} ���ز������, ����ֵ����:
     *    1: �����ɹ�
     *    0: key������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-22
     */
    int dump(const std::string& key, std::string& result);

    /**
     * @description
     *   �ж�key�Ƿ����. ֧��redis 3.0.3���ϰ汾.
     * @param [IN] vkeys {const std::vector<std::string>&} ������key
     * @return {int} ���ش��ڵ�key�ĸ���, ����ֵ����:
     *   >0: ���ڵ�key�ĸ���, ���������ͬһ��key���ظ����, �򽫻ᱻ��μ���
     *    0: key������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int exists(const std::vector<std::string>& vkeys);

    /**
     * @description
     *   �ж�key�Ƿ����.
     * @param [IN] key {const std::string&} ������key
     * @return {int} ����key�Ƿ����, ����ֵ����:
     *    1: key����
     *    0: key������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int exists(const std::string& key);

    /**
     * @description
     *   ����key������ʱ��. ����ʱ�䳬ʱ, key���ᱻɾ��
     * @param [IN] key {const std::string&} ������key
     * @param [IN] second {int} ����ʱ��, ��λ: ��. ���ʱ��Ϊ������, key������
     *   ��ɾ��
     * @return {int} ����ֵ����:
     *    1: ���óɹ�
     *    0: key�����ڻ�����ʱ���޷�������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int expire(const std::string& key, int second);

    /**
     * @description
     *   ��UNIXʱ�������key������ʱ��.
     * @param [IN] key {const std::string&} ������key
     * @param [IN] timestamp {time_t} UNIXʱ���. ���ʱ�����ʾ��ȥ��ʱ��, key
     *   ��������ɾ��
     * @return {int} ����ֵ����:
     *    1: ���óɹ�
     *    0: key�����ڻ�ʱ����޷�������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int expireat(const std::string& key, time_t timestamp);

    /**
     * @description
     *   �������з��ϸ���ģʽpattern������key.
     *   ֧�ֵ�������ģʽ:
     *    a) * ƥ������key
     *    b) h?llo ƥ�� hello, hallo �� hxllo
     *    c) h*llo ƥ�� hllo �� heeeello
     *    d) h[ae]llo ƥ�� hello �� hallo, ���ǲ�ƥ�� hillo
     *    e) h[^e]llo ƥ�� hallo, hbllo, ... ���ǲ�ƥ�� hello
     *    f) h[a-b]llo ƥ�� hallo �� hbllo
     * @param [IN] pattern {const std::string&} ƥ��ģʽ
     * @param [OUT] result {std::vector<std::string>&} �洢�����
     * @return {int} ���ط���������key�ĸ���, ����ֵ����:
     *   >=0: ����������key�ĸ���
     *    -1: ����
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int keys(const std::string& pattern, std::vector<std::string>& result);

    /**
     * @description
     *   ��ָ�����ݴӵ�ǰ��redis-serverʵ��Ǩ�Ƶ�Ŀ��redis-serverʵ����ָ������
     *   ����. �ò�����һ��ԭ�Ӳ���, �������ɹ�ʱ, ����ȷ�����ݴ�ԭʵ��ɾ������
     *   ����Ŀ��ʵ����.
     *   ѡ��:
     *      COPY - ���ݲ���ԭʵ��ɾ��.
     *      REPLACE - ����Ŀ��ʵ�����Ѿ����ڵ�ͬ����ֵ
     *      KEYS - ���key����Ϊ���ַ���, ������һ�ο���Ǩ�ƶ��key, Ǩ�Ƶļ�ֵ
     *       ��keys����ָ��
     * @param [IN] host {const std::string&} Ŀ��ʵ���ķ�������ַ.
     * @param [IN] port {uint16_t} Ŀ��ʵ���ķ���˿�.
     * @param [IN] key {const std::string&} ��ҪǨ�������ݵļ�ֵ. ȡֵΪ "" ʱ,
     *   ��ʾʹ�� KEYS ѡ��.
     * @param [IN] dest_db {int} Ŀ��ʵ�������ݿ�ID��.
     * @param [IN] timeout {long long} Ǩ�ƹ��̵ĳ�ʱʱ��, ��λ: ����.
     * @param [IN] params {MigrateParams*} migrate ѡ��
     * @return {bool} Ǩ���Ƿ�ɹ�.
     * @author chen.tengfei
     * @date 2017-04-26
     */
    bool migrate(const std::string& host, uint16_t port, const std::string& key,
        int dest_db, long long timeout, MigrateParams* params = NULL);

    /**
     * @description
     *   ��ָ��key�ӵ�ǰ���ݿ��ƶ���ָ�����ݿ�. �������ֹ�ڼ�Ⱥģʽ��ʹ��.
     * @param [IN] key {const std::string&} ָ���ƶ��ļ�ֵ
     * @param [IN] db {int} Ŀ�����ݿ�
     * @return {int} �Ƿ��ƶ��ɹ�, ����ֵ����:
     *    1: Ǩ�Ƴɹ�
     *    0: δǨ�Ƴɹ�, Ŀ�����ݿ�����ͬ���ֵĸ���key,��key�������ڵ�ǰ���ݿ���
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-26
     */
    int move(const std::string& key, int db);

    /**
     * @description
     *   ����ָ��key������Value�����õĴ���. ��������Ҫ���ڵ���
     * @param [IN] key {const std::string&} ָ���ļ�ֵ
     * @return {int} �������ô���, ����ֵ����:
     *   >0: ���ô���
     *    0: key������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-05-05
     */
    int object_refcount(const std::string& key);

    /**
     * @description
     *   ����ָ��key������Value���ڲ���ʾ��ʽ(��ѹ����ʽ).
     * @param [IN] key {const std::string&} ָ���ļ�ֵ
     * @param [OUT] encode {std::string&} ��ŷ��ؽ��(ѹ����ʽ), ��ֵ��ʾkey��
     *   ����.
     * @return {bool} ���ز������, ����ֵ����:
     *    true: �����ɹ�
     *   false: ����ʧ��
     * @author chen.tengfei
     * @date 2017-05-05
     */
    bool object_encoding(const std::string& key, std::string& encode);

    /**
     * @description
     *   ����ָ��key������Value�Ŀ���ʱ��(û�ж�д����), ��λ: ��
     * @param [IN] key {const std::string&} ָ���ļ�ֵ
     * @return {long long} ���ؿ���ʱ��, ����ֵ����:
     *   >0: ����ʱ��
     *    0: key������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-05-05
     */
    long long object_idletime(const std::string& key);

    /**
     * @description
     *   �Ƴ�ָ��key�ĳ�ʱʱ��.
     * @param [IN] key {const std::string&} ָ���ļ�ֵ
     * @return {int} ���ز������, ����ֵ����:
     *    1: �ɹ��Ƴ�����ʱ��
     *    0: key�����ڻ���keyû����������ʱ��
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-05-09
     */
    int persist(const std::string& key);

    /**
     * @description
     *   ����key������ʱ��. ����ʱ�䳬ʱ, key���ᱻɾ��
     * @param [IN] key {const std::string&} ������key
     * @param [IN] milliseconds {long long} ����ʱ��, ��λ: ����. ���ʱ��Ϊ����
     *   ��, key��������ɾ��
     * @return {int} ����ֵ����:
     *    1: ���óɹ�
     *    0: key�����ڻ�����ʱ���޷�������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int pexpire(const std::string& key, long long milliseconds);

    /**
     * @description
     *   ��UNIXʱ�������key������ʱ��.
     * @param [IN] key {const std::string&} ������key
     * @param [IN] timestamp {time_t} �Ժ���Ϊ��λ��UNIXʱ���. ���ʱ�����ʾ��
     *   ȥ��ʱ��, key��������ɾ��
     * @return {int} ����ֵ����:
     *    1: ���óɹ�
     *    0: key�����ڻ�ʱ����޷�������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int pexpireat(const std::string& key,
        unsigned long long milliseconds_timestamp);

    /**
     * @description
     *   ����ָ��keyʣ�������ʱ��, ��λ: ����
     * @param [IN] key {const std::string&} ������key
     * @return {long long} ����ʣ������ʱ��, ����ֵ����:
     *   >=0: ʣ������ʱ��
     *    -1: key���ڵ����Ѿ�����
     *    -2: key������
     * @author chen.tengfei
     * @date 2017-05-09
     */
    long long pttl(const std::string& key);

    /**
     * @description
     *   �ӵ�ǰ���ݿⷵ��һ�������key.
     * @param [OUT] out {std::string&} �洢���ص�key
     * @return {int} ���ز������, ����ֵ����:
     *    1: �ɹ�����һ�������key
     *    0: ��ǰ���ݿ�Ϊ��
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-05-09
     */
    int randomkey(std::string& out);

    /**
     * @description
     *   ��key������Ϊnew_key. ���key������, ��ʧ��; ���new_key����, ��ֵ����
     *   ����.
     * @param [IN] key {const std::string&} ������key
     * @param [IN] new_key {const std::string&} �µļ���
     * @return {bool} ���ز������, ����ֵ����:
     *    true: �����ɹ�
     *   false: ����ʧ��
     * @author chen.tengfei
     * @date 2017-05-09
     */
    bool rename(const std::string& key, const std::string& new_key);

    /**
     * @description
     *   ��new_key������ʱ, ��key������Ϊnew_key. ���key������, ��ʧ��.
     * @param [IN] key {const std::string&} ������key
     * @param [IN] new_key {const std::string&} �µļ���
     * @return {int} ���ز������, ����ֵ����:
     *    1: key �ɹ�����Ϊ new_key
     *    0: new_key �Ѿ�����
     *   -1: key ���ܴ���
     * @author chen.tengfei
     * @date 2017-05-09
     */
    int renamenx(const std::string& key, const std::string& new_key);

    /**
     * @description
     *   �����л����������л�ֵ, ��������� key ��. ��� key �Ѿ�����, �򷵻�һ
     *   ��"Target key name is busy"�Ĵ���, ����ʹ�� REPLACE ģʽ.
     * @param [IN] key {const std::string&} ������key.
     * @param [IN] TTL {unsigned long long} key������ʱ��, ��λ: ����.
     *   ���ttlΪ0, ����������ʱ��.
     * @param [IN] serialized_value {const std::string&} ���л���ֵ.
     * @param [IN] is_replace {bool} �Ƿ�ʹ�� REPLACE ģʽ(redis 3.0�����ϰ汾).
     *    true: ʹ�� REPLACE ģʽ.
     *   false: ��ʹ�� REPLACE ģʽ.
     * @return {bool} ���ز������, ����ֵ����:
     *    true: �����ɹ�
     *   false: ����ʧ��
     * @author chen.tengfei
     * @date 2017-05-09
     */
    bool restore(const std::string& key, unsigned long long TTL,
        const std::string& serialized_value, bool is_replace = false);

    /**
     * @description
     *   ���ڵ�����ǰѡ���redis���ݿ���key�ļ���
     * @param [IN] cursor {int} �α�ֵ, ��һ�ε���ʹ��0��Ϊ�α�.
     * @param [OUT] result {std::vector<std::string>&} �洢�����, �ڲ���׷�ӷ�
     *   ʽ�����α��������ӽ��ö�����, Ϊ��ֹ���ܽ���������¸��������, ��
     *   �����ڵ��ñ�����ǰ������ö���.
     * @param [IN] pattern {const char*} glob����ģʽ����, �ǿ�ʱ��Ч
     * @param [IN] count {int} �޶������Ԫ�ص�����, Ĭ��ֵ:10
     * @return {int} ��һ���α�λ��, ����ֵ����:
     *   >0: ��һ���α�λ��
     *    0: ��������
     *   -1: key��value���ʹ���(non-hash)
     */
    int scan(int cursor, std::vector<std::string>& result,
        const char* pattern = NULL, int count = 10);

    /**
     * @description
     *   ��key������list, set, zset�е�Ԫ������, ��������洢��ָ����dest��.
     *   Ĭ�ϰ�����ֵ��������, �Ƚϵ�Ԫ�ر�����˫���ȸ���������.
     * @param [IN] key {const std::string&} ������key.
     * @param [IN] dest {const std::string&} �洢������б��ֵ.
     * @param [IN] param {SortParams*} �������
     * @return {int} ���ؽ���б�ĳ���, ����ֵ����:
     *  >=0: ����б�ĳ���
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-05-09
     */
    int sort(const std::string& key, const std::string& dest,
        SortParams* params = NULL);

    /**
     * @description
     *   ����key������list, set, zset�е�������Ԫ��.Ĭ�ϰ�����ֵ��������, �Ƚ�
     *   ��Ԫ�ر�����˫���ȸ���������.
     * @param [IN] key {const std::string&} ������key.
     * @param [IN] result {std::vector<std::string>&} �洢������б�.
     * @param [IN] param {SortParams*} �������
     * @return {int} ���ؽ���б�ĳ���, ����ֵ����:
     *  >=0: ����б�ĳ���
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-05-09
     */
    int sort(const std::string& key, std::vector<std::string>& result,
        SortParams* params = NULL);

    /**
     * @description
     *   �ı����key���������ʱ��, ���Բ����ڵ�key.
     * @param [IN] vkeys {const std::vector<std::string>&} ָ����һ��keys
     * @return {int} ���ر�touched��key�ĸ���, ����:
     *   0: û��key�յ�Ӱ��
     *  >0: ʵ���ܵ�Ӱ���key�ĸ���, ��ֵ����С������key�ĸ���
     *  -1: ����
     * @author chen.tengfei
     * @date 2017-05-10
     */
    int touch(const std::vector<std::string>& vkeys);
    int touch(const std::string& key);

    /**
     * @description
     *   ����ָ��key��ʣ������ʱ��, ��λ: ��
     * @param [IN] key {const std::string&} ������key
     * @return {long long} ����ʣ������ʱ��, ����ֵ����:
     *   >=0: ʣ������ʱ��
     *    -1: key���ڵ����Ѿ�����
     *    -2: key������
     * @author chen.tengfei
     * @date 2017-05-10
     */
    long long ttl(const std::string& key);

    /**
     * @description
     *   ����key���洢��value�����ݽṹ����, ���Է���string, list, set, zset ��
     *   hash�Ȳ�ͬ������.
     * @param [IN] key {const std::string&} ������key
     * @return {std::string} ���ش洢�����ݽṹ����, Ϊ�ձ�ʾkey������.
     * @author chen.tengfei
     * @date 2017-05-10
     */
    std::string type(const std::string& key);

public:
    /***************************************************************************
     * server command
     ***************************************************************************/   
    /**
     * @description
     *   ������������ redis �첽ִ��һ�� AOF(AppendOnly File) �ļ���д����. ��д
     *   �����ᴴ��һ����ǰAOF�ļ���С���Ż��İ汾. ��ʹ BGREWRITEAOF ִ��ʧ��,
     *   Ҳ�������κ����ݶ�ʧ, ��Ϊ�ɵ�AOF�ļ��� BGREWRITEAOF �ɹ�֮ǰ���ᱻ�޸�.
     *   AOF ��д�� Redis ���д���, BGREWRITEAOF ���������ֶ�������д����.
     * @return {bool} ʼ�շ��� true
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool bgrewriteaof();

    /**
     * @description
     *   ��̨����DB. ���������� OK ״̬��. Redis forks, �����̼����ṩ�����Թ���
     *   ���˵���, �ӽ��̽�DB���ݱ��浽����Ȼ���˳�. ��������ɹ�, ����ͨ���ͻ�
     *   ������ LASTSAVE �����������.
     * @return {std::string} 
     * @author chen.tengfei
     * @date 2017-06-07
     */
    std::string bgsave();

    /**
     * @description
     *   ���ص�ǰ������ CLIENT SETNAME ���õ�����. ���û���� CLIENT SETNAME ��
     *   ������, ������һ���յĻظ�.
     * @param [OUT] name {std::string&} ��ǰ���ӵ�����.
     * @return {bool} ���ز������.
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool clientGetname(std::string& name);

    /**
     * @description
     *   �ر�һ��ָ��������. �رշ�ʽ����:
     *     CLIENT KILL ip:port
     *   ip:port��CLIENT LIST ���������г��Ŀͻ�������֮һ, ƥ�� addr ��
     * @param [IN] addr {const std::string&} �ͻ��˵�ַ�ͽӿ�
     * @return {bool} ���ز������, ����ֵ����:
     *    true: �ͻ��˴��ڲ����ѱ��ر�
     *   false: �ͻ��˲����ڻ����
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool clientKill(const std::string& addr);

    /**
     * @description
     *   �رշ���ָ�� filter �Ŀͻ���.
     * @param [IN] params {KillFilterParams} �ͻ��˵Ĳ�ͬ���Բ���.
     * @return {int} ���ر��رյĿͻ��˵�����.
     * @author chen.tengfei
     * @date 2017-06-07
     */
    int clientKill(KillFilterParams params);

    /**
     * @description
     *   �����������ӵ��������Ŀͻ�����Ϣ��ͳ������.     
     * @param [OUT] list {std::vector<std::map<std::string, std::string>*>*}
     *   ������Ŀͻ�����Ϣ��ͳ�������б�, �б��е�ÿ��Ԫ���ɵ����߸����ͷ�.
     * @return {std::string} ������Ϣ�ĸ�ʽ����:
     *   1. ÿ�ж�Ӧһ�������ӵĿͻ���(��LF�ָ�)
     *   2. ÿ���ַ�����һϵ�� ����=ֵ(property=value)��ʽ�������, ÿ����֮����
     *      �ո�ֿ�.
     *   ������Ϣ�����ֶεĺ�����ο�: https://redis.io/commands/client-list
     * @author chen.tengfei
     * @date 2017-06-11
     */
    std::string clientList(
        std::vector<std::map<std::string, std::string>*>* list = NULL);

    /**
     * @description
     *   ��ͣ���еĿͻ���, ֱ����ʱ. ������ִ�����²���:
     *     1. ֹͣ normal �� pub/sub �ͻ������������ڵȴ�ִ�еĹ�������, ���ǽ�
     *        ��ʽ�� slave �ͻ��˲��ᱻ��ͣ.
     *     2. ���ø�����Ŀͻ��˲�����ͣ, ���������� OK.
     *     3. ����ʱʱ���ѹ�, ���еĿͻ��˽��ᱻ�������: �⽫�ᴥ������ͣ�ڼ���
     *        ����ÿһ���ͻ��˵Ĳ�ѯ������(query buffer)�е����������ִ�й���.
     * @param [IN] timeout {long long} ָ���ĳ�ʱʱ��.
     * @return {bool} ����true.
     * @author chen.tengfei
     * @date 2017-06-13
     */
    bool clientPause(long long timeout);

    /**
     * @description
     *   ���� redis�������ظ��ͻ��˵�ģʽ. ģʽ����:
     *   ON: Ĭ��ģʽ, ������Ϊÿһ�������һ���ظ�;
     *   OFF: ��ģʽ��, ����������ظ��κοͻ�������;
     *   SKIP: ���������ŵ���һ������Ļظ�.
     * @param [IN] reply {CLIENT_REPLY} 
     * @return {int} ����ִ��״̬:
     *    1: ����Ϊ ON
     *    0: ����Ϊ OFF��SKIP
     * @author chen.tengfei
     * @date 2017-06-13
     */
    int clientReply(CLIENT_REPLY reply);

    /**
     * @description
     *   Ϊ��ǰ��������������, ���õ����ֿ��� CLIENT LIST ����鿴.
     * @param [IN] connName {const std::string&} 
     * @return {bool} ���óɹ�����true.
     * @author chen.tengfei
     * @date 2017-06-13
     */
    bool clientSetname(const std::string& connName);

    /**
     * @description
     *   ɾ���������ݿ��е���������, ��������Զ����ʧ��.
     * @return {bool}
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool flushAll();

    /**
     * @description
     *   ɾ����ǰ���ݿ��е���������, ��������Զ����ʧ��.
     * @return {bool}
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool flushDB();

    /**
     * @description
     *   �������һ�����ݼ��������ִ�гɹ�ʱ��ʱ���. �ͻ��˿���ͨ�����·�ʽ��
     *   �� BGSAVE �����Ƿ�ִ�гɹ�:
     *    1. ��ȡ LASTSAVE �ķ���ֵ
     *    2. ִ�� BGSAVE ����
     *    3. ÿ������鿴 LASTSAVE �ķ���ֵ�Ƿ����仯 
     * @return {long long} ���һ�����ݼ��������ִ�гɹ�ʱ��ʱ���.
     * @author chen.tengfei
     * @date 2017-06-07
     */
    long long lastsave();

    /**
     * @description
     *   ��ͬ����ʽ�������ݼ�, ����һ�� redis ʵ�����������ݵ�ʱ������, ������
     *   �� RDB �ļ���
     * @return {bool} ʼ�շ��� true
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool save();

public:
    /***************************************************************************
     * connection command
     ***************************************************************************/
    /**
     * @description
     *   ���м��ܱ����� redis-server �����������֤. redis ���������ڿͻ���ִ��
     *   commands����ǰ��Ҫͨ��������֤. ͨ���޸������ļ��� requirepass ָ��Ϳ�
     *   ����������.
     * @param [IN] password {const std::string} �� redis �����ļ���ָ������֤����
     * @return {bool}
     *    true: ����ƥ��, ����˿��Խ��պ�������
     *   false: ƥ��ʧ��, �ͻ�����Ҫ�����µ�����
     * @author chen.tengfei
     * @date 2017-06-06
     */
    bool auth(const std::string password);

    /**
     * @description
     *   ����˽��᷵�� message.
     * @param [IN] message {const std::string} 
     * @return {std::string} ����˷��ص���Ϣ
     * @author chen.tengfei
     * @date 2017-06-06
     */
    std::string echo(const std::string message);

    /**
     * @description
     *   ���������������һ�������Ƿ��ǿ��õ�, ������������һ�����ӵ���ʱ.
     * @param [IN] message {const std::string*} ���͸�����˵���Ϣ, Ϊ��ʱ, ����
     *   �˷��� PONG
     * @return {std::string} ����˷��ص���Ϣ
     * @author chen.tengfei
     * @date 2017-06-06
     */
    std::string ping(const std::string* message);
    bool ping();

    /**
     * @description
     *   ����������ر�����.
     * @return {bool} ʼ�շ��� true.
     * @author chen.tengfei
     * @date 2017-06-06
     */
    bool quit();

    /**
     * @description
     *   ѡ��һ�����ݿ�. ���ݿ��±��0��ʼ. �Ǽ�Ⱥģʽ��serverʵ����16�����ݿ�,
     *   ��1~15. ��Ⱥģʽ��serverʵ��ֻ��һ�����ݿ�. ����������ڼ�Ⱥģʽ��ʹ
     *   ��.
     * @param [IN] index {int} ���ݿ��±�, ����0
     * @return {bool} ���ز������, true��ʾ���ݿ��л��ɹ�
     * @author chen.tengfei
     * @date 2017-06-04
     */
    bool select(int index);

private:
    RedisClient* m_client;
};

#endif /* __REDIS_H__ */

