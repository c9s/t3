#ifndef _H_THREADPOOL
#define _H_THREADPOOL

#include <thread>
#include <mutex>
#include <queue>
#include <deque>
#include <vector>
#include <condition_variable>
#include "thread_task.h"

const int DEFAULT_POOL_SIZE = 10;
const int STARTED = 0;
const int STOPPED = 1;

class ThreadPool
{

public:


    ThreadPool();

    ThreadPool(int pool_size);

    ~ThreadPool();

    void Initialize();

    void Destroy();

    void ExecuteTask();

    void AddTask(ThreadTask* task);

private:

    int started_workers_ = 0;

    int pool_size_;

    // The mutex for locking task queue
    std::mutex mutex_;

    // condition variable to wake up threads
    std::condition_variable cv_;

    // pool for thread objects
    std::vector<std::thread*> threads_; // storage for threads

    // the task queue
    std::deque<ThreadTask*> tasks_;

    volatile int state_;
};

#endif /* _H_THREADPOOL */
