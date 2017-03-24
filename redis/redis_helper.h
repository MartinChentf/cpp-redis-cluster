#ifndef __REDIS_HELPER_H__
#define __REDIS_HELPER_H__

#include <sstream>
#include <string>
#include <vector>
#include <map>

class redis_helper
{
public:

    /**
     * �Ѹ���ֵת�����ַ���
     */
    template<typename T>
    static std::string to_string(T a) {
        std::ostringstream ss;
        ss << a;
        return ss.str();
    }

    /**
     * �Ѹ������ַ����б�ƴװ�ɵ����ַ���
     * @param  [IN] list {const std::vector<std::string>&} �ַ����б�
     * @param  [IN] separator {const std::string} �ָ���, Ĭ��ֵΪ�ո�
     * @return {std::string} ƴװ����ַ���
     */
    static std::string join(const std::vector<std::string>& list, const std::string& separator = " ");

    /**
     * �Ѹ�����map�ļ�ֵ��ƴװ�ɵ����ַ���
     * @param  [IN] map {const std::map<std::string, std::string>&} ������map
     * @param  [IN] separator_key_value {const std::string&} ����ֵ֮��ķָ���, Ĭ��Ϊ�ո�
     * @param  [IN] separator {const std::string&} ��ֵ��֮��ķָ���, Ĭ��ֵΪ�ո�
     * @return {std::string} ƴװ����ַ���
     */
    static std::string join(const std::map<std::string, std::string>& map,
                            const std::string& separator_key_value = " ",
                            const std::string& separator = " ");
};

static const char * const REPLY_TYPE[] =
{
    "",
    "REDIS_REPLY_STRING",   // 1
    "REDIS_REPLY_ARRAY",    // 2
    "REDIS_REPLY_INTEGER",  // 3
    "REDIS_REPLY_NIL",      // 4
    "REDIS_REPLY_STATUS",   // 5
    "REDIS_REPLY_ERROR"     // 6
};

#define SAFE_ASSIGN(p, val) if (p) (*p) = (val)

#define SAFE_ASSIGN_FUNC(p, func) if (p) (*p) = (func); else (void)(func)

#endif /* __REDIS_HELPER_H__ */
