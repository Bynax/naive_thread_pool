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
    int m_pool_size;
    Mutex m_task_mutex;
    CondVar m_task_cond_var;
    vector<pthread_t> m_threads;
    deque<Task*> m_tasks;

    volatile int m_pool_state;
};


#endif //NAIVE_THREAD_POOL_THREADPOOL_H
