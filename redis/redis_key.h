#ifndef __REDIS_KEY_H__
#define __REDIS_KEY_H__

#include <vector>
#include <string>

#include "redis_command.h"

class redis_key : public redis_command
{
public:
    redis_key(redis_client* client):redis_command(client){}
    ~redis_key(){}

public:
    /**
     * ɾ��ָ����һ����һ��keys, ���Բ����ڵ�key
     * @param [IN] key {const std::vector<std::string>&} ָ����һ��keys
     * @return {int} ����ʵ��ɾ����key�ĸ���, ����:
     *   0: δɾ���κ�key
     *  >0: ʵ��ɾ����key�ĸ���, ��ֵ����С������key�ĸ���
     *  -1: ����
     */
    int del(const std::vector<std::string>& keys);
    int del(const std::string key);
};

#endif
