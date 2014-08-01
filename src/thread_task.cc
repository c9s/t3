/*
 * thread_task.cc
 * Copyright (C) 2014 c9s <c9s@c9smba.local>
 *
 * Distributed under terms of the MIT license.
 */
#include <thread>
#include <mutex>
#include <condition_variable>
#include "thread_task.h"

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

void ThreadTask::set_done(bool f) {
    done_ = f;
}

void ThreadTask::Wait()
{
    {
        std::unique_lock<std::mutex> lock(mutex);
        while (!done_) {
            cond.wait(lock);
        }
    }
    cond.notify_one();
}






