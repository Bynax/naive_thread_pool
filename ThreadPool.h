/********************************************************************************* 
  * FileName: ThreadPool.h
  * Date 2020/3/14
  * Description: 
**********************************************************************************/

#ifndef NAIVE_THREAD_POOL_THREADPOOL_H
#define NAIVE_THREAD_POOL_THREADPOOL_H

#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <deque>

#include <iostream>
#include "Task.h"
#include "Mutex.h"
#include "Global.h"
#include "CondVar.h"
using namespace std;

class ThreadPool {
public:
    ThreadPool();
    explicit ThreadPool(int pool_size);
    ~ThreadPool();
    int initial_threadpool();
    int destroy_threadpool();

    void* execute_thread();
    int add_task(Task* task);

private:
    int m_pool_size; // 线程池数量
    Mutex m_task_mutex; // 互斥锁，用来保护条件变量
    CondVar m_task_cond_var; // 条件变量，同步任务队列
    vector<pthread_t> m_threads; // 存放thread的容器
    deque<Task*> m_tasks; // 任务队列

    volatile int m_pool_state; // 线程池的状态 start or stop，因为要保证各个线程可以看到最新的状态，因此使用volatile
};


#endif //NAIVE_THREAD_POOL_THREADPOOL_H
