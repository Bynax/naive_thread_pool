/********************************************************************************* 
  * FileName: ThreadPool.cpp
  * Date 2020/3/14
  * Description: 
**********************************************************************************/

#include "ThreadPool.h"

ThreadPool::ThreadPool() :
        m_pool_size(DEFAULT_POOL_SIZE) {
}

ThreadPool::ThreadPool(int pool_size) :
        m_pool_size(pool_size) {
}

ThreadPool::~ThreadPool() {
    // 释放资源
    if (m_pool_state != STOPPED) {
        destroy_threadpool();
    }
}

extern "C" {
    void *start_thread(void *args) {
        ThreadPool* tp = (ThreadPool *) args;
        tp->execute_thread();
        return NULL;
    }
}

int ThreadPool::initial_threadpool() {

    m_pool_state = STARTED;
    int ret = -1;
    for (int i = 0; i < m_pool_size; i++) {
        pthread_t tid;
        ret = pthread_create(&tid, NULL,start_thread,(void*) this);
        if(ret != 0){
            cerr<<"pthread_create fialed: "<<ret<<"\n";
            return -1;
        }
        m_threads.push_back(tid);
        cout<<"threads creates by the thread pool\n";
    }
    return 0;
}

int ThreadPool::destroy_threadpool() {
    m_task_mutex.lock();
    m_pool_state = STOPPED;
    m_task_mutex.unlock();
    cout<<"Broadcasting STOP signal to all threads\n";
    m_task_cond_var.broadcast();

    int ret = -1;
    for(int i = 0; i < m_pool_size; i++){
        void* result;
        ret = pthread_join(m_threads[i],&result);
        cout<<"pthread_join returned "<< ret <<":"<<
        strerror(errno)<<"\n";
        m_task_cond_var.broadcast();
    }
    cout<<m_pool_size<<" threads exited from the thread pool\n";
    return 0;
}

void *ThreadPool::execute_thread() {
    Task* task = NULL;
    cout<<"Starting thread "<<pthread_self()<<"\n";
    while(true){
        cout<<"Locking "<<pthread_self()<<"\n";
        m_task_mutex.lock();

        while((m_pool_state != STOPPED) &&
                (m_tasks.empty())){
            cout<<"Unlocking and waiting: "<<pthread_self()<<"\n";
            m_task_cond_var.wait(m_task_mutex.get_mutex_ptr());
            cout<<"Signaled and locking: "<<pthread_self()<<"\n";
        }
        if(m_pool_state == STOPPED){
            cout<<"Unlocking and exiting: "<<pthread_self()<<"\n";
            m_task_mutex.unlock();
            pthread_exit(NULL);
        }
        task = m_tasks.front();
        m_tasks.pop_front();
        cout<<"Unlocking: "<<pthread_self()<<"\n";
        m_task_mutex.unlock();

        (*task)();
        delete task;
    }
    return NULL;
}

int ThreadPool::add_task(Task *task) {
    m_task_mutex.lock();

    m_tasks.push_back((task));

    m_task_cond_var.signal();

    m_task_mutex.unlock();
    return 0;
}