cmake_minimum_required(VERSION 3.15)
project(naive_thread_pool)

set(CMAKE_CXX_STANDARD 11)

add_executable(naive_thread_pool main.cpp ThreadPool.cpp ThreadPool.h Task.cpp Task.h Mutex.cpp Mutex.h CondVar.cpp CondVar.h Global.h)