/**
 * Copyright 2017-2022 chen.tengfei <chentf1836@163.com>
 * All rights reserved.
 *
 * @file   Util.h
 * @brief 
 * @version 1.0
 * @bug none
 * @warning none
 * @other none
 * @author chen.tengfei
 * @data   2017-06-11 23:16:27
 */
#ifndef __UTIL_H__
#define __UTIL_H__

#include <sstream>
#include <string>
#include <vector>
#include <map>

class Util
{
public:

    /**
     * 把给定值转换成字符串
     */
    template<typename T>
    static std::string to_string(T a) {
        std::ostringstream ss;
        ss << a;
        return ss.str();
    }

    static std::string to_string(std::string str) { return str; }

    template<typename T, typename U>
    static std::string to_string(std::pair<T, U> pair,
                                 const std::string& separator = ":") {
        return to_string(pair.first) + separator + to_string(pair.second);
    }

    /**
     * 把给定的列表拼装成单个字符串
     * @param  [IN] list {const std::vector<T>&} 给定的列表
     * @param  [IN] separator {const std::string} 分隔符, 默认值为空格
     * @return {std::string} 拼装后的字符串
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

     template<typename T>
    static std::string join(const std::vector<T*>& list,
                            const std::string& separator = " ") {
        std::string str("");
        for (size_t i = 0; i < list.size(); i++) {
            str += list[i] ? to_string(*list[i]) : ""
                   + ((i<list.size()-1) ? separator : "");
        }
    
        return str;
    }

    /**
     * 把给定的map的键值对拼装成单个字符串
     * @param  [IN] map {const std::map<K, V>&} 给定的map
     * @param  [IN] separator_key_value {const std::string&}
     *   键和值之间的分隔符, 默认为空格
     * @param  [IN] separator {const std::string&}
     *   键值对之间的分隔符, 默认值为空格
     * @return {std::string} 拼装后的字符串
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

    /**
     * @description
     *   把一个字符串分割成字符串数组.
     * @param [IN] str {const std::string&} 被分割的字符串.
     * @param [IN] separator {const std::string&} 分隔符.
     * @param [OUT] list {std::vector<std::string>&} 分割后的字符串数组
     * @return {void} 
     * @author chen.tengfei
     * @date 2017-06-11
     */
    static void split(const std::string& str, const std::string& separator,
        std::vector<std::string>& list);
};

#define SAFE_ASSIGN(p, val) do {                                                \
    if (p)                                                                      \
        (*p) = (val);                                                           \
} while (0)

#define SAFE_ASSIGN_FUNC(p, func) do {                                          \
    if (p)                                                                      \
        (*p) = (func);                                                          \
    else                                                                        \
        (void)(func);                                                           \
} while (0)

#define SAFE_DELETE(p) do {                                                     \
    if (p) {                                                                    \
        delete (p);                                                             \
        (p) = NULL;                                                             \
    }                                                                           \
} while (0)

#define TO_STRING(a) Util::to_string(a)

#endif /* __UTIL_H__ */
