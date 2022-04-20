#ifndef TINYWEBSERVER_SQL_CONNECTION_POOL_H
#define TINYWEBSERVER_SQL_CONNECTION_POOL_H
#include <iostream>
#include <string>
#include <list>
#include <mysql/mysql.h>
using namespace std;

class sql_connection_pool
{
public:
    MYSQL *mysql;                        //获取数据库连接
    bool ReleaseConnection(MYSQL *conn); //释放连接
    int GetFreeConn();                   //获取连接
    void DestroyPool();                  //销毁所有连接

    //单例模式
    static sql_connection_pool *GetInstance();

    void init(string url, string User, string PassWord, string DataBaseName, int Port, int MaxConn, int close_log);

private:
    sql_connection_pool();
    ~sql_connection_pool();

    int m_MaxConn;
    int m_CurConn;
    int m_FreeConn;
    list<MYSQL *> connList;

public:
    string m_url;
    string m_User;
    string m_Port;
    string m_PassWord;
    string m_DatabaseName;
    int m_close_log;
};

class connectionRAII
{
public:
    connectionRAII(MYSQL **conn, sql_connection_pool *connPool);
    ~connectionRAII();

private:
    MYSQL *conRAII;
    sql_connection_pool *poolRAII;
};

#endif // TINYWEBSERVER_SQL_CONNECTION_POOL_H
