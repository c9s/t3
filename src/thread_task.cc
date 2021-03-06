/*
 * thread_task.cc
 * Copyright (C) 2014 c9s <c9s@c9smba.local>
 *
 * Distributed under terms of the MIT license.
 */
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "thread_task.h"

namespace t3 {

ThreadTask::ThreadTask(void (*fn_ptr)(ThreadTask*, void*), void* arg) : function_(fn_ptr), arguments_(arg)
{

}

ThreadTask::~ThreadTask()
{
}

void ThreadTask::operator()()
{
    (*function_)(this, arguments_);
}

void ThreadTask::Run()
{
    (*function_)(this, arguments_);
}

void ThreadTask::Done(bool f) {
    std::unique_lock<std::mutex> lock(mutex);
    done_ = f;
    cond.notify_all();
}

void ThreadTask::Wait()
{
    std::unique_lock<std::mutex> lock(mutex);
    while (!done_) {
        cond.wait(lock);
    }
}





};
