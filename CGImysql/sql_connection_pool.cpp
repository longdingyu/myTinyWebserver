#include "sql_connection_pool.h"

using namespace std;

sql_connection_pool::sql_connection_pool()
{
    m_CurConn = 0;
    m_FreeConn = 0;
}

sql_connection_pool::~sql_connection_pool()
{
}
//单例模式
sql_connection_pool *sql_connection_pool::GetInstance()
{
    static sql_connection_pool connPool;
    return &connPool;
}

void sql_connection_pool::init(string url, string User, string PassWord, string DataBaseName, int Port, int MaxConn,
                               int close_log)
{
    m_url = url;
    m_User = User;
    m_PassWord = PassWord;
    m_DatabaseName = DataBaseName;
    m_Port = Port;
    m_MaxConn = MaxConn;
    m_close_log = close_log;

    cout << "-- sql_connection_pool is ok --" << endl;

    for (int i = 0; i < MaxConn; i++)
    {
        //        MYSQL* con = NULL;
    }
}
