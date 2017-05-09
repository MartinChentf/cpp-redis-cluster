#ifndef __REDIS_REPLY_H__
#define __REDIS_REPLY_H__

#include <vector>
#include <string>

typedef enum {
    REDIS_REPLY_UNKOWN,     // 0
    REDIS_REPLY_STRING,     // 1
    REDIS_REPLY_ARRAY,      // 2
    REDIS_REPLY_INTEGER,    // 3
    REDIS_REPLY_NIL,        // 4
    REDIS_REPLY_STATUS,     // 5
    REDIS_REPLY_ERROR,      // 6
} t_redis_reply;

static const char * const REPLY_TYPE[] =
{
    "REDIS_REPLY_UNKOWN",   // 0
    "REDIS_REPLY_STRING",   // 1
    "REDIS_REPLY_ARRAY",    // 2
    "REDIS_REPLY_INTEGER",  // 3
    "REDIS_REPLY_NIL",      // 4
    "REDIS_REPLY_STATUS",   // 5
    "REDIS_REPLY_ERROR"     // 6
};

/**
 * @description
 *   �洢����������� redis-server Ӧ��������. �����߰ѽ������ redis-server
 *   Ӧ�����ݴ��� redis_reply �����.
 * @author chen.tengfei
 * @date 2017-04-22
 */
class redis_reply
{
public:
    redis_reply();
    ~redis_reply();

public:
    /**
     * @description
     *   ���ص�ǰ redis_reply �ڵ����������
     * @return {t_redis_reply} 
     * @author chen.tengfei
     * @date 2017-04-23
     */
    t_redis_reply get_type() const { return m_type; }

    /**
     * @description
     *   ���ص�ǰ�ڵ�洢�� redis_reply ����ĸ���
     * @return {size_t} ����ֵ��ڵ��������͵Ķ�Ӧ��ϵ����:
     *   REDIS_REPLY_UNKOWN: -1
     *   REDIS_REPLY_NIL: 0
     *   REDIS_REPLY_ERROR: 1
     *   REDIS_REPLY_STATUS: 1
     *   REDIS_REPLY_STRING: 1
     *   REDIS_REPLY_INTEGER: 1
     *   REDIS_REPLY_ARRAY: �ӽڵ�ĸ���
     * @author chen.tengfei
     * @date 2017-04-23
     */
    size_t get_size() const;

    /**
     * @description
     *   ������������ REDIS_REPLY_ARRAY ����ʱ, ����ָ���±���ӽڵ����.
     * @param [IN] idx {size_t} �±�ֵ
     * @return {const redis_reply*} ����±�Խ����߲������ӽڵ�, ���ؿ���
     * @author chen.tengfei
     * @date 2017-04-23
     */
    const redis_reply* get_element(size_t idx) const;

    /**
     * @description
     *   ������������ REDIS_REPLY_STATUS ����ʱ, ���ص�ǰ�ڵ��״̬��Ϣ
     * @return {std::string} ��ǰ�ڵ��״̬��Ϣ, ���ؿ��ַ�����ʾ�������Ͳ�ƥ��
     * @author chen.tengfei
     * @date 2017-04-23
     */
    std::string get_status() const;

    /**
     * @description
     *   ������������ REDIS_REPLY_ERROR ����ʱ, ���ص�ǰ�ڵ�Ĵ�����Ϣ
     * @return {std::string} ��ǰ�ڵ�Ĵ�����Ϣ, ���ؿ��ַ�����ʾ�������Ͳ�ƥ��
     * @author chen.tengfei
     * @date 2017-04-23
     */
    std::string get_error() const;

    /**
     * @description
     *   ������������ REDIS_REPLY_STRING ����ʱ, ���ص�ǰ�ڵ���ַ�����Ϣ
     * @return {std::string} ��ǰ�ڵ�洢���ַ���
     * @author chen.tengfei
     * @date 2017-04-23
     */
    std::string get_string() const { return m_str; }

    /**
     * @description
     *   ������������ REDIS_REPLY_INTEGER ����ʱ, ���ص�ǰ�ڵ�洢�ĳ���������
     * @return {long long} ��ǰ�ڵ�洢�ĳ���������
     * @author chen.tengfei
     * @date 2017-04-23
     */
    long long get_integer() const { return m_integer; }

    /**
     * @description
     *   ������������ REDIS_REPLY_STRING ����ʱ, �����ַ�����Ӧ��˫���ȸ�����
     * @param [OUT] success {bool*} ��ָ��ǿ�ʱ, �洢��������Ƿ�ɹ�
     * @return {double} ˫���ȸ�����ֵ
     * @author chen.tengfei
     * @date 2017-04-23
     */
    double get_double(bool* success = NULL) const;

public:
    /**
     * @description
     *   ���õ�ǰredis_reply�ڵ����������
     * @param [IN] type {t_redis_reply} �ڵ����������
     * @return {redis_reply&} ��ǰ�ڵ��thisָ��
     * @author chen.tengfei
     * @date 2017-04-23
     */
    redis_reply& set_type(t_redis_reply type);

    /**
     * @description
     *   ��ǰ�ڵ�������. ��ǰ�ڵ�Ϊ�� REDIS_REPLY_ARRAY ����ʱʹ��.
     *   ע��: ���ñ�����ǰ�����ȵ��� set_type ����ָ����ȷ����������, �������
     *         ����ɴ洢���ݻ���
     * @param [IN] buff {const std::string&} 
     * @return {redis_reply&} ��ǰ�ڵ��thisָ��
     * @author chen.tengfei
     * @date 2017-04-23
     */
    redis_reply& put(const std::string& buff);
    redis_reply& put(const char* buff, int len);

    /**
     * @description
     *   ��ǰ�ڵ�������. ��ǰ�ڵ�Ϊ REDIS_REPLY_ARRAY ����ʱʹ��.
     *   ע��: ���ñ�����ǰ�����ȵ��� set_type ����ָ����ȷ����������, �������
     *         ����ɴ洢���ݻ���
     * @param [IN] rr {const redis_reply*} 
     * @return {redis_reply&} ��ǰ�ڵ��thisָ��
     * @author chen.tengfei
     * @date 2017-04-23
     */
    redis_reply& put(const redis_reply* rr);

private:
    void clear();

private:
    // REDIS_REPLY_*
    t_redis_reply m_type;
    // The integer when type is REDIS_REPLY_INTEGER
    long long m_integer;
    // Used for both REDIS_REPLY_ERROR and REDIS_REPLY_STRING
    std::string m_str;
    // elements vector for REDIS_REPLY_ARRAY
    std::vector<const redis_reply*> m_element;
};

#endif /* __REDIS_REPLY_H__ */
