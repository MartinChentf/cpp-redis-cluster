#ifndef __REDIS_LOG_H__
#define __REDIS_LOG_H__

#include <string>

class redis_log
{
private:
    redis_log();
    ~redis_log();

public:
    static redis_log* get_instance()
    {
        static redis_log _log;
        return &_log;
    }

    void log(const char* file_name, int line, const int level, const char* format, ...);
    

private:
    // ½ûÖ¹¿½±´ºÍ¸³Öµ
    redis_log(const redis_log&);
    redis_log& operator=(const redis_log&);

    std::string get_level_name(int level);
};

#define LOG_LEVEL_NORMAL    0
#define LOG_LEVEL_DEBUG     1
#define LOG_LEVEL_WARN      2
#define LOG_LEVEL_ERROR     3

#define NORMAL(...)         redis_log::get_instance()->log(__FILE__, __LINE__, LOG_LEVEL_NORMAL, __VA_ARGS__)
#define DEBUG(...)          redis_log::get_instance()->log(__FILE__, __LINE__, LOG_LEVEL_NORMAL, __VA_ARGS__)
#define WARN(...)           redis_log::get_instance()->log(__FILE__, __LINE__, LOG_LEVEL_NORMAL, __VA_ARGS__)
#define ERROR(...)          redis_log::get_instance()->log(__FILE__, __LINE__, LOG_LEVEL_NORMAL, __VA_ARGS__)


#endif /* __REDIS_LOG_H__ */
