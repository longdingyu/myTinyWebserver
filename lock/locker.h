#ifndef TINYWEBSERVER_LOCKER_H
#define TINYWEBSERVER_LOCKER_H
/*
 * 线程同步机制包装类
 * 多线程同步，确保任一时刻只能有一个线程能进入关键代码段
 * */
#include <pthread.h>
#include <semaphore.h>
#include <exception>

//锁
class locker
{
public:
    locker()
    {
        if (pthread_mutex_init(&m_mutex, NULL) != 0)
        {
            throw std::exception();
        }
    }

    ~locker()
    {
        pthread_mutex_destroy(&m_mutex);
    }

    bool lock()
    {
        return pthread_mutex_lock(&m_mutex) == 0;
    }

    bool unlock()
    {
        return pthread_mutex_unlock(&m_mutex) == 0;
    }

    pthread_mutex_t *get()
    {
        return &m_mutex;
    }

private:
    pthread_mutex_t m_mutex;
};

//信号量
class sem
{
public:
    sem()
    {
        if (sem_init(&m_sem, 0, 0) != 0)
        {
            throw std::exception();
        }
    }

    sem(int num)
    {
        if (sem_init(&m_sem, 0, num) != 0)
        {
            throw std::exception();
        }
    }

    ~sem()
    {
        sem_destroy(&m_sem);
    }

    bool wait()
    {
        return sem_wait(&m_sem) == 0;
    }

    bool post()
    {
        return sem_post(&m_sem) == 0;
    }

private:
    sem_t m_sem;
};

//条件变量
class cond
{
public:
    cond()
    {
        if (pthread_cond_init(&m_cond, NULL) != 0)
        {
            throw std::exception();
        }
    }

    ~cond()
    {
        pthread_cond_destroy(&m_cond);
    }

    bool wait(pthread_mutex_t *m_mutex)
    {
        int ret = 0;
        //        pthread_mutex_lock(&m_mutex);
        ret = pthread_cond_wait(&m_cond, m_mutex);
        //        pthread_mutex_unlock(&m_mutex);
        return ret == 0;
    }

    bool timedwait(pthread_mutex_t *m_mutex, struct timespec t)
    {
        int ret = 0;
        //        pthread_mutex_lock(&m_mutex);
        ret = pthread_cond_timedwait(&m_cond, m_mutex, &t);
        //        pthread_mutex_unlock(&m_mutex);
        return ret == 0;
    }

    bool signal()
    {
        return pthread_cond_signal(&m_cond);
    }

    bool broadcost()
    {
        return pthread_cond_broadcast(&m_cond);
    }

private:
    //    static pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
};

#endif // TINYWEBSERVER_LOCKER_H
