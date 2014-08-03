/*
 * channel.h
 * Copyright (C) 2014 c9s <c9s@c9smba.local>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CHANNEL_H
#define CHANNEL_H

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

namespace t3 {

template<typename T> class Channel {
public:
    std::mutex mutex_;
    std::queue<T> queue_;
    std::condition_variable cond;

    bool done;
    Channel() : done(false) { }

    void push(T t);
    T pop();
    bool empty();
};

template<typename T> inline bool Channel<T>::empty() {
    return queue_.empty();
}

template<typename T> inline void Channel<T>::push(T t) { 
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(t);
    cond.notify_one();
}

template<typename T> inline T Channel<T>::pop() {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty()) {
        cond.wait(mlock);
    }
    T t = queue_.front();
    queue_.pop();
    return t;
}

};
#endif /* !CHANNEL_H */
