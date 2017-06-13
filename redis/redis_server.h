#ifndef __REDIS_SERVER_H__
#define __REDIS_SERVER_H__

#include <string>
#include <vector>
#include <map>

#include "ProtocolCommand.h"
#include "redis_command.h"

class KillFilterParams;

class redis_server : virtual public redis_command
{
public:
    redis_server(const std::string& host, uint16_t port);
    ~redis_server(){}

public:
    /**
     * @description
     *   ������������ redis �첽ִ��һ�� AOF(AppendOnly File) �ļ���д����. ��д
     *   �����ᴴ��һ����ǰAOF�ļ���С���Ż��İ汾. ��ʹ BGREWRITEAOF ִ��ʧ��,
     *   Ҳ�������κ����ݶ�ʧ, ��Ϊ�ɵ�AOF�ļ��� BGREWRITEAOF �ɹ�֮ǰ���ᱻ�޸�.
     *   AOF ��д�� Redis ���д���, BGREWRITEAOF ���������ֶ�������д����.
     * @return {bool} ʼ�շ��� true
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool bgrewriteaof();

    /**
     * @description
     *   ��̨����DB. ���������� OK ״̬��. Redis forks, �����̼����ṩ�����Թ���
     *   ���˵���, �ӽ��̽�DB���ݱ��浽����Ȼ���˳�. ��������ɹ�, ����ͨ���ͻ�
     *   ������ LASTSAVE �����������.
     * @return {std::string} 
     * @author chen.tengfei
     * @date 2017-06-07
     */
    std::string bgsave();

    /**
     * @description
     *   ���ص�ǰ������ CLIENT SETNAME ���õ�����. ���û���� CLIENT SETNAME ��
     *   ������, ������һ���յĻظ�.
     * @param [OUT] name {std::string&} ��ǰ���ӵ�����.
     * @return {bool} ���ز������.
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool clientGetname(std::string& name);

    /**
     * @description
     *   �ر�һ��ָ��������. �رշ�ʽ����:
     *     CLIENT KILL ip:port
     *   ip:port��CLIENT LIST ���������г��Ŀͻ�������֮һ, ƥ�� addr ��
     * @param [IN] addr {const std::string&} �ͻ��˵�ַ�ͽӿ�
     * @return {bool} ���ز������, ����ֵ����:
     *    true: �ͻ��˴��ڲ����ѱ��ر�
     *   false: �ͻ��˲����ڻ����
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool clientKill(const std::string& addr);

    /**
     * @description
     *   �رշ���ָ�� filter �Ŀͻ���.
     * @param [IN] params {KillFilterParams} �ͻ��˵Ĳ�ͬ���Բ���.
     * @return {int} ���ر��رյĿͻ��˵�����.
     * @author chen.tengfei
     * @date 2017-06-07
     */
    int clientKill(KillFilterParams params);

    /**
     * @description
     *   �����������ӵ��������Ŀͻ�����Ϣ��ͳ������.     
     * @param [OUT] list {std::vector<std::map<std::string, std::string>*>*}
     *   ������Ŀͻ�����Ϣ��ͳ�������б�, �б��е�ÿ��Ԫ���ɵ����߸����ͷ�.
     * @return {std::string} ������Ϣ�ĸ�ʽ����:
     *   1. ÿ�ж�Ӧһ�������ӵĿͻ���(��LF�ָ�)
     *   2. ÿ���ַ�����һϵ�� ����=ֵ(property=value)��ʽ�������, ÿ����֮����
     *      �ո�ֿ�.
     *   ������Ϣ�����ֶεĺ�����ο�: https://redis.io/commands/client-list
     * @author chen.tengfei
     * @date 2017-06-11
     */
    std::string clientList(
        std::vector<std::map<std::string, std::string>*>* list = NULL);

    /**
     * @description
     *   ��ͣ���еĿͻ���, ֱ����ʱ. ������ִ�����²���:
     *     1. ֹͣ normal �� pub/sub �ͻ������������ڵȴ�ִ�еĹ�������, ���ǽ�
     *        ��ʽ�� slave �ͻ��˲��ᱻ��ͣ.
     *     2. ���ø�����Ŀͻ��˲�����ͣ, ���������� OK.
     *     3. ����ʱʱ���ѹ�, ���еĿͻ��˽��ᱻ�������: �⽫�ᴥ������ͣ�ڼ���
     *        ����ÿһ���ͻ��˵Ĳ�ѯ������(query buffer)�е����������ִ�й���.
     * @param [IN] timeout {long long} ָ���ĳ�ʱʱ��.
     * @return {bool} ����true.
     * @author chen.tengfei
     * @date 2017-06-13
     */
    bool clientPause(long long timeout);

    /**
     * @description
     *   ���� redis�������ظ��ͻ��˵�ģʽ. ģʽ����:
     *   ON: Ĭ��ģʽ, ������Ϊÿһ�������һ���ظ�;
     *   OFF: ��ģʽ��, ����������ظ��κοͻ�������;
     *   SKIP: ���������ŵ���һ������Ļظ�.
     * @param [IN] reply {CLIENT_REPLY} 
     * @return {int} ����ִ��״̬:
     *    1: ����Ϊ ON
     *    0: ����Ϊ OFF��SKIP
     * @author chen.tengfei
     * @date 2017-06-13
     */
    int clientReply(CLIENT_REPLY reply);

    /**
     * @description
     *   Ϊ��ǰ��������������, ���õ����ֿ��� CLIENT LIST ����鿴.
     * @param [IN] connName {const std::string&} 
     * @return {bool} ���óɹ�����true.
     * @author chen.tengfei
     * @date 2017-06-13
     */
    bool clientSetname(const std::string& connName);

    /**
     * @description
     *   ɾ���������ݿ��е���������, ��������Զ����ʧ��.
     * @return {bool}
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool flushAll();

    /**
     * @description
     *   ɾ����ǰ���ݿ��е���������, ��������Զ����ʧ��.
     * @return {bool}
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool flushDB();

    /**
     * @description
     *   �������һ�����ݼ��������ִ�гɹ�ʱ��ʱ���. �ͻ��˿���ͨ�����·�ʽ��
     *   �� BGSAVE �����Ƿ�ִ�гɹ�:
     *    1. ��ȡ LASTSAVE �ķ���ֵ
     *    2. ִ�� BGSAVE ����
     *    3. ÿ������鿴 LASTSAVE �ķ���ֵ�Ƿ����仯 
     * @return {long long} ���һ�����ݼ��������ִ�гɹ�ʱ��ʱ���.
     * @author chen.tengfei
     * @date 2017-06-07
     */
    long long lastsave();

    /**
     * @description
     *   ��ͬ����ʽ�������ݼ�, ����һ�� redis ʵ�����������ݵ�ʱ������, ������
     *   �� RDB �ļ���
     * @return {bool} ʼ�շ��� true
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool save();

};

/**
 * ���� CLIENT KILL ����� filter ����
 */
class KillFilterParams
{
public:
    std::vector<std::string>& getParams() { return params; }

    /**
     * @description
     *   �ر� addr ����ָ����һ������.
     *   addr ����ƥ�� CLIENT LIST �����ֵ��addr ��.
     * @param [IN] addr {const std::string&} �ͻ��˵�ַ�ͽӿ�, ��ʽ�� ip:port.
     * @return {KillFilterParams&} ���� KillFilterParams �����.
     * @author chen.tengfei
     * @date 2017-06-08
     */
    KillFilterParams& ADDR(const std::string& addr);

    /**
     * @description
     *   ͨ���ͻ��˵�ΨһID��ر�һ������.
     *   id ����ƥ�� CLIENT LIST �����ֵ��id ��.
     * @param [IN] id {int} �ͻ��˵�ΨһID.
     * @return {KillFilterParams&} ���� KillFilterParams �����.
     * @author chen.tengfei
     * @date 2017-06-08
     */
    KillFilterParams& ID(int id);

    /**
     * @description
     *   �ر�ָ���ͻ������͵���������. �ͻ������������¼���:
     *    normal : TYPE_NORMAL
     *    master : TYPE_MASTER, �� v3.2 �汾��ʼ����
     *    slave  : TYPE_SLAVE
     *    pubsub : TYPE_PUBSUB
     *   ��ʾ: �� MONITOR ���������Ŀͻ��˱���Ϊ���� normal ����
     * @param [IN] type {CLIENT_TYPE} �ͻ�������
     * @return {KillFilterParams&} ���� KillFilterParams �����.
     * @author chen.tengfei
     * @date 2017-06-08
     */
    KillFilterParams& TYPE(CLIENT_TYPE type);

    /**
     * @description
     *   ��ѡ��Ĭ��ֵ��yes, ��ʾ���ø�����Ŀͻ��˲��ᱻ�ر�, �����ѡ�����ó�no,
     *   ��ʾ���ø�����Ŀͻ���Ҳ�ᱻ�ر�.
     * @param [IN] yes {bool} 
     * @return {KillFilterParams&} ���� KillFilterParams �����.
     * @author chen.tengfei
     * @date 2017-06-08
     */
    KillFilterParams& SKIPMME(bool yes = true);

private:
    std::vector<std::string> params;
};

#endif /* __REDIS_SERVER_H__ */
