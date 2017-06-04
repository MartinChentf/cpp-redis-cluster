#ifndef __GT_COMMON_H__
#define __GT_COMMON_H__

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>

#define LINE_MAX_CHARS 1000

class str_func
{
public:
    /**
     * @description
     *   ɾ���ַ������ߵĿ��ַ�, �հ׷�ָ��Ϊ�ո�(' '), ��λ�ַ�('\t'),
     *   CR('\r'), ����('\n'), ��ֱ��λ�ַ�('\v')��ҳ('\f')
     * @param [IN/OUT] src {char*} Դ�ַ���
     * @return {char*} ȥ�����߿��ַ���srcָ��
     */
    static char* trim(char* src);
    static char* left_trim(char* src);
    static char* right_trim(char* src);
};

class readini
{
public:
    readini(const char* filename);
    ~readini();

public:    
    bool initialize();
    char* readstring(const char *Section, const char *Ident, char *sItem,
                     size_t iSize, const char *defstr );
    int   readinteger(const char *Section, const char *Ident,
                      const int defint );
    long  readinteger(const char *Section, const char *Ident,
                      const long deflong );

private:
    FILE *m_fd;
    std::string m_filename;

private:
    readini(const readini&);
    readini& operator=(const readini&);
};

class gt_config
{
public:
    gt_config(const char* config_filename);

private:
    void read_config(const char* config_filename);

public:
    std::vector< std::pair<std::string, uint16_t> > m_single;

    std::vector< std::pair<std::string, uint16_t> > m_cluster;
};

class gt_component
{
private:
    gt_component():m_config("./gtest/gtest.config"){}
    ~gt_component(){};

private:
    gt_component(const gt_component&);
    gt_component& operator=(const gt_component&);

public:
    static inline gt_component& Instance( )
    {
        static gt_component m_instance;
        return m_instance;
    }

    std::string get_host(int pos);
    uint16_t get_port(int pos);

    std::string get_cluster_host(int pos);
    uint16_t get_cluster_port(int pos);

private:
    gt_config m_config;
};

#endif /* __GT_COMMON_H__ */
