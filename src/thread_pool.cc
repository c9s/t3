#include "thread_pool.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <errno.h>

namespace t3 {

ThreadPool::ThreadPool() : pool_size_(DEFAULT_POOL_SIZE) {
    Initialize();
}

ThreadPool::ThreadPool(int pool_size) : pool_size_(pool_size) {
    Initialize();
}

ThreadPool::~ThreadPool()
{
    // Release resources
    if (state_ != STOPPED) {
        Destroy();
    }
}

void ThreadHandler(ThreadPool * pool)
{
    pool->ExecuteTask();
}

void ThreadPool::Initialize()
{
    // TODO: Consider lazy loading threads instead of creating all at once
    state_ = STARTED;
    for (int i = 0; i < pool_size_; i++) {
        auto t = new std::thread(ThreadHandler, this);
        threads_.push_back(t);
        // we need to make sure that threads has entered the section to waiting the lock 
    }

    // Wait for all threads get started.
    std::unique_lock<std::mutex> lock(mutex_);
    while (started_workers_ < pool_size_) {
        cv_.wait(lock);
    }
}

void ThreadPool::Destroy()
{
    // Note: this is not for synchronization, its for thread communication!
    // destroy() will only be called from the main thread, yet
    // the modified state_ may not show up to other threads until its 
    // modified in a lock!
    mutex_.lock();
    state_ = STOPPED;
    cv_.notify_all(); // notify all threads we are shttung down
    mutex_.unlock();
    // cout << "Broadcasting STOP signal to all threads..." << endl;

    for (std::vector<std::thread*>::iterator it = threads_.begin(); it != threads_.end(); it++ ) {
        if ( (*it)->joinable()) {
            (*it)->join();
        }
        cv_.notify_all(); // try waking up a bunch of threads that are still waiting
    }
}

void ThreadPool::ExecuteTask()
{
    ThreadTask* task = NULL;

    {
        std::unique_lock<std::mutex> lock(mutex_);
        started_workers_++;
        cv_.notify_all(); // try waking up a bunch of threads that are still waiting
    }

    while(true) {
        std::unique_lock<std::mutex> mlock(mutex_);
        while ((state_ != STOPPED) && (tasks_.empty())) {
            cv_.wait(mlock);
        }

        if (state_ == STOPPED) {
            return;
        }

        // take one task and pop the front
        task = tasks_.front();
        tasks_.pop_front();
        mlock.unlock();

        if (task) {
            task->set_pool(this);
            task->Run();
            task->Done();
        }
    }
}

void ThreadPool::AddTask(ThreadTask* task)
{
    std::unique_lock<std::mutex> mlock(mutex_);
    tasks_.push_back(task);
    cv_.notify_one(); // wake up one thread that is waiting for a task to be available
}

};
