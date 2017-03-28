#ifndef __REDIS_HASH_H__
#define __REDIS_HASH_H__

#include <map>
#include <string>
#include <vector>

#include "redis_command.h"

class redis_client;

class redis_hash : public redis_command
{
public:
    typedef std::map<std::string, std::string> t_field_value_map;
    typedef t_field_value_map::iterator t_field_value_map_iter;

public:
    redis_hash(redis_client* client):redis_command(client){}
    ~redis_hash(){}

public:
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
     * @return {bool} �����Ƿ�ɹ�, false��ʾ�����key��value���ʹ���(non-hash)
     */
    bool hgetall(const std::string& key,
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
};

#endif /* __REDIS_HASH_H__ */
