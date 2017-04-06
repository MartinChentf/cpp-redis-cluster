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

    static std::string to_string(std::string str) { return str; }

    /**
     * �Ѹ������б�ƴװ�ɵ����ַ���
     * @param  [IN] list {const std::vector<T>&} �������б�
     * @param  [IN] separator {const std::string} �ָ���, Ĭ��ֵΪ�ո�
     * @return {std::string} ƴװ����ַ���
     */
     template<typename T>
    static std::string join(const std::vector<T>& list,
                            const std::string& separator = " ") {
        std::string str("");
        for (size_t i = 0; i < list.size(); i++) {
            str += to_string(list[i])
                   + ((i<list.size()-1) ? separator : "");
        }
    
        return str;
    }

    /**
     * �Ѹ�����map�ļ�ֵ��ƴװ�ɵ����ַ���
     * @param  [IN] map {const std::map<K V>&} ������map
     * @param  [IN] separator_key_value {const std::string&}
     *   ����ֵ֮��ķָ���, Ĭ��Ϊ�ո�
     * @param  [IN] separator {const std::string&}
     *   ��ֵ��֮��ķָ���, Ĭ��ֵΪ�ո�
     * @return {std::string} ƴװ����ַ���
     */
    template<typename K, typename V>
    static std::string join(const std::map<K, V>& map,
                            const std::string& separator_key_value = " ",
                            const std::string& separator = " ") {
        std::string str("");
        typename std::map<K,V>::const_iterator it = map.begin();
        typename std::map<K,V>::const_iterator last = map.end();
        last--;
        for (; it != map.end(); ++it) {
            str += to_string(it->first) + separator_key_value
                   + to_string(it->second) + ((it != last) ? separator : "");
        }

        return str;
    }
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
