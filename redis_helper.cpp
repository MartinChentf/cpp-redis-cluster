#include "redis_helper.h"
#include "redis_log.h"


static string RedisHelper::generateCommand(string cmd,const char * format,...)
{
    va_list ap;
    string command = ;
    va_start(ap,format);
    vsnprintf();
    va_end(ap);    
}

