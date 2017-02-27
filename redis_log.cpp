#include <stdio.h>

#include "redis_log.h"

void redis_log::log(const char * file_name,int line,const int level,const char * format,...)
{
    const char log_tmp[10240] = {0};
    va_list ap;
    va_start (ap, format);
    vsnprintf(log_tmp, sizeof(log_tmp), format, ap);
    va_end (ap);

    printf("%s[%s:%d] %s\n", get_level_name(level).c_str(), file_name, line, log_tmp);
}

std::string redis_log::get_level_name(int level)
{
    switch(level) {
        case LOG_LEVEL_NORMAL:
            return std::string("[NORMAL]");
        case LOG_LEVEL_DEBUG:
            return std::string("[ DEBUG]");
        case LOG_LEVEL_WARN:
            return std::string("[  WARN]");
        case LOG_LEVEL_ERROR:
            return std::string("[ ERROR]");
        default:
            return std::string("[ (N/A)]");
    }
}

