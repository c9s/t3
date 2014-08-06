/*
 * thread_task.h
 * Copyright (C) 2014 c9s <c9s@c9smba.local>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef THREAD_TASK_H
#define THREAD_TASK_H

#include <thread>
#include <mutex>
#include <condition_variable>

namespace t3 {

class ThreadPool;

class ThreadTask
{

public:
    bool done_ = false;
    std::mutex mutex;
    std::condition_variable cond;

    float total_ = 0;

    float finished_ = 0;

    ThreadPool * pool_;

    //  ThreadTask(TCLass::* obj_fn_ptr); // pass an object method pointer
    ThreadTask(void (*fn_ptr)(ThreadTask*, void*), void* arg); // pass a free function pointer

    ~ThreadTask();

    ThreadPool * pool() const { return pool_; };

    void set_pool(ThreadPool * pool) { pool_ = pool; }

    void operator()();

    void Wait();

    // Set done_ flag to done
    void Done(bool f = true);

    void Run();


    // Set total progress for the progress report.
    void SetTotalProgress(float total) {
        total_ = total;
    }

    // Set finished report
    void UpdateProgress(float finished) {
        finished_ = finished;
    }

    // Return the progress report in float (finished_ / total_)
    float ReadProgress() {
        return (float)finished_ / (float)total_;
    }

private:
    void (*function_)(ThreadTask* task, void* data);
    void * arguments_;
};

};

#endif /* !THREAD_TASK_H */
