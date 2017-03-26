#ifndef __REDIS_STRING_H__
#define __REDIS_STRING_H__

#include <string>
#include <vector>
#include <map>

#include "redis_command.h"

class redis_client;

class redis_string : public redis_command
{
public:
    enum BITOP {
        AND,
        OR,
        NOT,
        XOR,

        SIZE_BITOP
    };
    static const char* BITOP_STR[SIZE_BITOP];

public:
    redis_string(redis_client* client):redis_command(client){}
    ~redis_string(){}

public:
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
    long long bitconut(const std::string& key);
    long long bitconut(const std::string& key, int start, int end);

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
     *   ��keyֵ�����ڷ��ؿ�.
     * @param [IN] keys {const std::vector<std::string>&}
     *   һ���ַ��������key
     * @param [OUT] result {std::vector<std::string*>&}
     *   keys��������value, ��Ҫ�������ֶ��ͷ��ڴ�
     * @return {bool} ����true��ʾִ�гɹ�, �õ��ò���ʧ��
     */
    bool mget(const std::vector<std::string>& keys,
              std::vector<std::string*>& result);
    bool mget(const std::vector<std::string>& keys,
              std::vector<std::string*>* result);

    /**
     * @description
     *   ��key��������Ӧ��value��, �Ḳ��ԭ�е�valueֵ, ��Ϊ��set����.
     * @param [IN] keyValues {const std::map<std::string, std::string>&} ��ֵ��
     * @return {bool} ����true��ʾִ�гɹ�, �õ��ò���ʧ��
     */
    bool mset(const std::map<std::string, std::string>& keyValues);

    /**
     * @description
     *   ��key��������Ӧ��value��, ֻҪ��һ��key����, ����msetnx������ִ��.
     * @param [IN] keyValues {const std::map<std::string, std::string>&} ��ֵ��
     * @return {bool} ����true��ʾִ�гɹ�, �õ��ò���ʧ��
     */
    bool msetnx(const std::map<std::string, std::string>& keyValues);

    /**
     * @description
     *   �Դ洢��ָ��key����ִֵ�м�1����. ���key������, ����ǰ��key��ֵ��Ϊ0
     * @param [IN] key {const std::string&} ָ��������key
     * @param [OUT] result {long long*} �ǿ�ʱ�洢�������
     * @return {bool} ��ʾ���������Ƿ�ɹ�, ����:
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
     * @return {bool} ��ʾ���������Ƿ�ɹ�, ����:
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
     * @return {bool} ��ʾ���������Ƿ�ɹ�, ����:
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
     * @return {bool} ��ʾ�Լ������Ƿ�ɹ�, ����:
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
     * @return {bool} ��ʾ�Լ������Ƿ�ɹ�, ����:
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
     * @return {bool} ��ʾ�Լ������Ƿ�ɹ�, ����:
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
     * @return {bool} �����Ƿ�ɹ�, ����:
     *    true: �����ɹ�
     *   false: key��value���ʹ���(non-string)
     */
    bool strlen(const std::string& key, long long& length);

private:
    bool set_string(const std::string& key, const std::string& value,
                    const char* ex_or_px, long long second,
                    const char* nx_or_xx);
    bool incoper(const char* cmd, const std::string& key,
                 long long* inc, long long* result);
};

#endif /* __REDIS_STRING_H__ */
