#include <iostream>

#include "ThreadPool.h"

using namespace std;

const int MAX_TASKS = 1000;

void hello(void* arg)
{
    int* x = (int*) arg;
    cout << "Hello " << *x << endl;
}

int main(int argc, char* argv[])
{
    ThreadPool tp(100);
    int ret = tp.initial_threadpool();
    if (ret == -1) {
        cerr << "Failed to initialize thread pool!" << endl;
        return 0;
    }

    for (int i = 0; i < MAX_TASKS; i++) {
        int* x = new int();
        *x = i+1;
        Task* t = new Task(&hello, (void*) x);
        tp.add_task(t);
    }

    sleep(2);

    tp.destroy_threadpool();


    cout << "Exiting app..." << endl;

    return 0;
}