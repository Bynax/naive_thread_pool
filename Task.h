/********************************************************************************* 
  * FileName: Task.h
  * Date 2020/3/15
  * Description: 
**********************************************************************************/

#ifndef NAIVE_THREAD_POOL_TASK_H
#define NAIVE_THREAD_POOL_TASK_H


#include <pthread.h>
#include <unistd.h>
#include <deque>
#include <iostream>
#include <vector>
#include <errno.h>
#include <string.h>

#include "Global.h"

using namespace std;

class Task
{
public:
    Task(void (*fn_ptr)(void*), void* arg); // pass a free function pointer
    ~Task();
    void operator ()();
    void run();
private:
    void (*m_fn_ptr)(void*);
    void* m_arg;
};


#endif //NAIVE_THREAD_POOL_TASK_H
