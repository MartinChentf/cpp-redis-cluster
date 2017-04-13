#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "redis_log.h"
#include "gt_common.h"

//=================================
//      str_func ����
//=================================
char* str_func::left_trim(char * src)
{
    ASSERT(src != NULL);
    char *pTemp = 0;
    if (*src == '\0')
        return src;
    int i = (int)strlen(src);

    pTemp = src;
    while (*pTemp)
    {
        if (!isspace(*pTemp))
            break;
        else
            pTemp++;
    }
    if (pTemp > src)
        memmove(src, pTemp, (size_t)(i - (pTemp - src) + 1));  //zmp:535842
    return src;
}

char* str_func::right_trim(char * src)
{
    ASSERT(src != NULL);

    char *pTemp = 0;
    if (*src == '\0')
        return src;
    pTemp = src + strlen(src) - 1;
    while (*pTemp)
    {
        if (!isspace(*pTemp))
            break;
        else
        {
            *pTemp = 0;
            pTemp--;
        }
    }
    return src;
}

char* str_func::trim(char * src)
{
    ASSERT(src != NULL);

    right_trim(src);
    left_trim(src);

    return src;
}

//=================================
//      readini ����
//=================================
readini::readini(const char * filename)
: m_fd(NULL)
, m_filename(filename)
{}

readini::~readini()
{
    if (m_fd) {
        fclose(m_fd);
        m_fd = NULL;
    }
}

bool readini::initialize()
{    
    m_fd = fopen(m_filename.c_str(), "r");
    if (!m_fd) {
        return false;
    }

    return true;
}

char * readini::readstring(const char *Section, const char *Ident,
                            char *sItem, size_t iSize, const char *defstr)
{
    char sBuffer[LINE_MAX_CHARS] = {0};
    char   sSect[LINE_MAX_CHARS] = {0};
    char  sIdent[LINE_MAX_CHARS] = {0};

    char *pos = 0;
    int  nStep = 0;  // 0:δ�ҵ�ָ��Section; 1:�ҵ�ָ��Section; 2:�ҵ�ָ��Ident

    // 1. ����Ĭ��ֵ
    if ( defstr == NULL ) {
        memccpy(sItem, "" , 0, iSize - 1);
        sItem[iSize-1] = 0;
    }
    else {
        memccpy(sItem, defstr, 0, iSize - 1);
        sItem[iSize-1] = 0;
    }

    // 2. �ļ�δ��, ֱ�ӷ���Ĭ��ֵ
    if (!m_fd) {
        return sItem;
    }

    // 3. fseekʧ��, ֱ�ӷ���Ĭ��ֵ
    if(fseek(m_fd, 0, SEEK_SET) != 0) {
        return sItem;
    }

    snprintf(sSect, sizeof(sSect), "[%s]", Section);

    // 4. ɨ���ļ�, ����ָ��Ident
    while (fgets(sBuffer, LINE_MAX_CHARS, m_fd) != NULL) {
        str_func::trim(sBuffer);

        // ���Կ��к�ע��
        if ((sBuffer[0] == '#') || (sBuffer[0] == '\n')) {
            continue;
        }

        // ȥ��ÿ��ĩβ��ע��
        char* pNewLine = strchr(sBuffer, '#');
        if( pNewLine != NULL ) {
            *pNewLine = '\0';
        }

        if (nStep == 0) {
            // 5.1 ����ָ��Section, ����ҵ�Section, ������һ��
            if (strstr(sBuffer, sSect)) {
                nStep = 1;
            }
            continue;
        }
        else if (nStep == 1) {
            // 5.2 ����ָ��Ident, ��������ɶԷ��������������
            bool bBreakFlag = strchr(sBuffer, '[') && strchr(sBuffer, ']');
            if (bBreakFlag) {
                break;
            }

            if ((pos = strchr(sBuffer, '=')) == NULL) {
                continue;
            }
            memccpy(sIdent, sBuffer, 0, (size_t)(pos - sBuffer));
            sBuffer[(size_t)(pos - sBuffer)] = 0;
            if (strcmp(str_func::trim(sIdent), Ident) == 0) {
                nStep = 2 ;
                break;
            }
        }
    }

    if (nStep == 2) {
        memccpy(sItem, str_func::trim(pos + 1), 0, iSize - 1);
        sItem[iSize-1] = 0;
    }
    else if ( defstr == NULL ) {
        return NULL;
    }

    return sItem;
}

int readini::readinteger(const char * Section, const char * Ident,
                         const int defint)
{
    char defstr[LINE_MAX_CHARS] = {0};
    char sItem[LINE_MAX_CHARS] = {0};
    char *ptr = NULL;

    snprintf(defstr, sizeof(defstr), "%d", defint);
    ptr = readstring(Section, Ident, sItem, sizeof(sItem), defstr);
    if ( ptr == NULL )
        return defint;
    else
        return atoi(sItem) ;
}

long readini::readinteger(const char * Section, const char * Ident,
                         const long deflong)
{
    char defstr[LINE_MAX_CHARS] = {0};
    char sItem[LINE_MAX_CHARS] = {0};
    char *ptr = NULL;

    snprintf(defstr, sizeof(defstr), "%ld", deflong);
    ptr = readstring(Section, Ident, sItem, sizeof(sItem), defstr);
    if ( ptr == NULL )
        return deflong;
    else
        return atol(sItem);
}

//=================================
//      gt_config ����
//=================================
gt_config::gt_config(const char * config_filename)
{
    read_config(config_filename);
}

void gt_config::read_config(const char * config_filename)
{
    ASSERT(config_filename);

    readini init_file(config_filename);
    if (!init_file.initialize()) {
        ASSERT("Read gtest.config fail!!!");
        return;
    }

    char sItem[LINE_MAX_CHARS];
    m_host = init_file.readstring("COMMON", "HOST", sItem, sizeof(sItem),
                                  "127.0.0.1");
    m_port = (uint16_t)init_file.readinteger("COMMON", "PORT", 6379);
}

