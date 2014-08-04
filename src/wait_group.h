/*
 * wait_group.h
 * Copyright (C) 2014 c9s <c9s@c9smba.local>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef WAIT_GROUP_H
#define WAIT_GROUP_H

#include <atomic>
#include <mutex>
#include <condition_variable>

namespace t3 {

    class WaitGroup {

        std::condition_variable cv_;
        std::mutex mutex_;

        std::atomic<int> counter_;
        std::atomic<int> waiters_;

    public:
        WaitGroup() : counter_(0) , waiters_(0) { }
        WaitGroup(int counter) : counter_(counter), waiters_(0) { }

        void Add(int n) {
            counter_ += n;
        }

        void Inc() {
            ++counter_;
        }

        void Done() { 
            std::unique_lock<std::mutex> lock(mutex_);
            --counter_;
            cv_.notify_one();
        }

        void Wait() {
            ++waiters_;
            std::unique_lock<std::mutex> lock(mutex_);
            while( counter_ > 0 ) {
                cv_.wait(lock);
            }
        }

        int Finished() {
            return counter_ == 0;
        }
    };
};


#endif /* !WAIT_GROUP_H */
