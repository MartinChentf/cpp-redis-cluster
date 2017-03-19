#ifndef __REDIS_LOG_H__
#define __REDIS_LOG_H__

#include <assert.h>
#include <string>

class redis_log
{
private:
    redis_log(){}
    ~redis_log(){}

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
#define DEBUG(...)          redis_log::get_instance()->log(__FILE__, __LINE__, LOG_LEVEL_DEBUG, __VA_ARGS__)
#define WARN(...)           redis_log::get_instance()->log(__FILE__, __LINE__, LOG_LEVEL_WARN, __VA_ARGS__)
#define ERROR(...)          redis_log::get_instance()->log(__FILE__, __LINE__, LOG_LEVEL_ERROR, __VA_ARGS__)

#define PRINT_COLOR_NONE        "\033[m"
#define PRINT_COLOR_BLACK       "\033[30m"
#define PRINT_COLOR_RED         "\033[31m"
#define PRINT_COLOR_GREED       "\033[32m"
#define PRINT_COLOR_YELLOW      "\033[33m"
#define PRINT_COLOR_BLUE        "\033[34m"
#define PRINT_COLOR_PURPLE      "\033[35m"
#define PRINT_COLOR_DARK_GREEN  "\033[36m"
#define PRINT_COLOR_WHITR       "\033[37m"

void LogAssertError(const char * exp);
#ifdef NDEBUG
#define ASSERT(exp) ((exp) ? (void)0 : LogAssertError(#exp))
#else
#define ASSERT(exp) assert(exp)
#endif

#endif /* __REDIS_LOG_H__ */
