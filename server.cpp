#include "server.h"
#include <cstring>
#include <assert.h>

Server::Server()
{
    users = new http_conn[MAX_FD];
    char server_path[200];
    // cout << "server_path = " << getcwd(server_path, 200) << endl;
    char root[6] = "/root";
    m_root = (char *)malloc(strlen(server_path) + strlen(root) + 1);
    strcpy(m_root, server_path);
    strcat(m_root, root);

    // cout << "m_root = " << m_root << endl;
}

Server::~Server()
{
    //    close(m_epollfd);
    //    close(m_listenfd);
    //    close(m_pipefd[1]);
    //    close(m_pipefd[2]);
    //    delete[] users;
    //    delete[] users_timer;
    //    delete m_pool;
}

void Server::init(int port, string user, string password, string databaseName, int log_write, int opt_linger, int trigmode,
                  int sql_num, int thread_num, int close_log, int actor_model)
{
    m_port = port;
    m_user = user;
    m_passWord = password;
    m_databaseName = databaseName;
    //    m_log_write = log_write;
    //    m_OPT_LINGER = opt_linger;
    //    m_TRIGMode = trigmode;
       m_sql_nums = sql_num;
    //    m_thread_num = thread_num;
    //    m_close_log = close_log;
    //    m_actormodel = actor_model;
}

void Server::thread_pool()
{
    m_pool = new threadpool<http_conn>(m_actormodel, m_connPool, m_thread_num);
}

void Server::sql_pool()
{
    //初始化数据库连接池
    m_connPool = sql_connection_pool::GetInstance();
    m_connPool->init("localhost", m_user, m_passWord, m_databaseName, 3306, m_sql_nums, m_close_log);

    //初始化数据库读取表
    //
}

void Server::log_write()
{
}

void Server::trig_mode()
{
}

void Server::eventListen()
{
    cout << "--listenfd is ok--" << endl;
    m_listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(m_listenfd >= 0);

    if (0 == m_OPT_LINGER)
    {
        struct linger tmp = {0, 1};
        setsockopt(m_listenfd, SOL_SOCKET, SOL_SOCKET, &tmp, sizeof(tmp));
    }
    else if (1 == m_OPT_LINGER)
    {
        struct linger tmp = {1, 1};
        setsockopt(m_listenfd, SOL_SOCKET, SOL_SOCKET, &tmp, sizeof(tmp));
    }

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(m_port);

    int flag = 1;
    setsockopt(m_listenfd, SOL_SOCKET, SOL_SOCKET, &flag, sizeof(flag));
    ret = bind(m_listenfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret >= 0);

    ret = listen(m_listenfd, 5);
    assert(ret >= 0);

    //    定时器初始化
    //    utils.init(TIMESLOT);

    //    epoll 创建内核事件
}

void Server::eventLoop()
{
}

// void Server::timer(int connfd, struct sockaddr_in client_address) {
//
// }
