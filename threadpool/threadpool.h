#ifndef TINYWEBSERVER_THREADPOOL_H
#define TINYWEBSERVER_THREADPOOL_H
/*
 * 使用一个工作队列完全解除了主线程和工作线程的耦合关系：主线程往工作队列中插入任务，工作线程通过竞争来取得任务并执行它。
 * 同步I/O模拟proactor模式
 * 半同步/半反应堆
 * 线程池
 * */

#include "../CGImysql/sql_connection_pool.h"
#include "../lock/locker.h"
#include <exception>
#include <pthread.h>
#include <list>

template <typename T>
class threadpool
{
public:
    threadpool(int actor_model, sql_connection_pool *connPool, int thread_number = 8, int max_request = 10000);
    ~threadpool();
    bool append(T *request, int state);
    bool append_p(T *request);

private:
    /*工作线程运行的函数，它不断从工作队列中取出任务并执行之*/
    static void *worker(void *arg);
    void run();

private:
    int m_thread_number;             //线程池中线程数
    int m_max_requests;              //请求队列最大请求数
    pthread_t *m_threads;            //描述线程池的数组，大小为m_thread_nubmer
    list<T *> m_workqueue;           //请求队列
    sem m_queuestat;                 //是否有任务需要处理
    locker m_queuelocker;            //保护请求队列的互斥锁
    sql_connection_pool *m_connPool; //数据库
    int m_actor_model;               //模式切换
};

template <typename T>
threadpool<T>::threadpool(int actor_model, sql_connection_pool *connPool, int thread_number, int max_request)
{
    #if 1
    cout << "-- threadpool is ok --" << endl;
    if (thread_number <= 0 || max_request <= 0)
    {
        throw std::exception();
    }
    m_threads = new pthread_t[m_thread_number];
    if (!m_threads)
        throw std::exception();
    for (int i = 0; i < thread_number; i++)
    {
        if (pthread_create(m_threads + 1, NULL, worker, this) != 0)
        {
            delete[] m_threads;
            throw std::exception();
        }
        if (pthread_detach(m_threads[i]))
        {
            delete[] m_threads;
            throw std::exception();
        }
    }
    #endif
}

template <typename T>
void *threadpool<T>::worker(void *arg)
{
    threadpool *pool = (threadpool*)arg;
    pool->run();
    return pool;
}

template <typename T>
void threadpool<T>::run()
{
    cout << "-- threadpool is run -- " << endl;
    while(true){
        m_queuestat.wait();
        m_queuelocker.lock();
        if(m_workqueue.empty()){
            m_queuelocker.unlock();
            continue;
        }
        T *request = m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelocker.unlock();
        if(!request) continue;
        if(1 == m_actor_model){
            if(0 == request->m_state)   //此处m_state
            {
                if(request->read_once()){
                    //此处重写
                }
            }
        }
    }
}

template<typename T>
threadpool<T>::~threadpool() {
    delete[] m_threads;
}

template<typename T>
bool threadpool<T>::append(T *request, int state) {
    m_queuelocker.lock();
    if(m_workqueue.size() >= m_max_requests)
    {
        m_queuelocker.unlock();
        return false;
    }
    request->m_state = state;
    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuestat.post();
    return true;
}

template<typename T>
bool threadpool<T>::append_p(T *request) {
    m_queuelocker.lock();
    if(m_workqueue.size() >= m_max_requests)
    {
        m_queuelocker.unlock();
        return false;
    }
    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuestat.post();

    return true;
}


#endif // TINYWEBSERVER_THREADPOOL_H
