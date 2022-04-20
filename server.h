#ifndef SERVER_H
#define SERVER_H
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "./http/http_conn.h"
#include "./CGImysql/sql_connection_pool.h"
#include "./threadpool/threadpool.h"
using namespace std;

const int MAX_FD = 65536;
const int MAX_EVENT_NUMBER = 10000;
const int TIMESLOT = 5;

class Server
{
public:
    Server();
    ~Server();

    void init(int port, string user, string password, string databaseName,
              int log_write, int opt_linger, int trigmode, int sql_num,
              int thread_num, int close_log, int actor_model);

    void thread_pool();
    void sql_pool();
    void log_write();
    void trig_mode();
    void eventListen();
    void eventLoop();
    void timer(int connfd, struct sockaddr_in client_address);
    // void adjust_timer(util_timer *timer);

public:
    int m_port;
    char *m_root;
    int m_log_write;
    int m_close_log;
    int m_actormodel;

    int m_pipefd[2];
    int m_epollfd;
    http_conn *users;

    //数据库相关
    sql_connection_pool *m_connPool;
    string m_user;
    string m_passWord;
    string m_databaseName;
    int m_sql_nums;

    //线程池相关
    threadpool<http_conn> *m_pool;
    int m_thread_num;

    // epoll_event 相关
    // epoll_event events[MAX_EVENT_NUMBER];

    int m_listenfd;
    int m_OPT_LINGER;
    int m_TRIGMode;
    int m_LISTENTrigmode;
    int m_CONNTrigmode;

    //定时器相关
    // client_data *users_timer;
    //    Utils utils;
};

#endif