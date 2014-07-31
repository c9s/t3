/*
 * thread_pool_test.cc
 * Copyright (C) 2014 c9s <c9s@c9smba.local>
 *
 * Distributed under terms of the MIT license.
 */
#include "thread_task.h"
#include "thread_pool.h"

#include <gperftools/profiler.h>
#include "gtest/gtest.h"

ThreadPool global_pool(4);

struct MutexData { 
    int * a;
    std::mutex mutex;
    MutexData(int * a) : a(a) { }

    void Inc() {
        *a = *a + 1;
    }
};

TEST(ThreadPool, TestThreadConstructorAndDestructor) {
    ThreadPool pool(4);
}

void inc_a(ThreadTask *task, void * data);
void inc_a_500(ThreadTask *task, void * data);
void parent_a(ThreadTask *task, void * data);


void parent_a(ThreadTask *task, void * data) {
    int * a = (int*) data;

    *a = *a + 1;

    MutexData tdata(a);

    ThreadTask task1(inc_a, &tdata);
    ThreadTask task2(inc_a, &tdata);

    global_pool.AddTask(&task1);
    global_pool.AddTask(&task2);

    task1.Wait();
    task2.Wait();

    task->Done();
}

void inc_a_500(ThreadTask *task, void * data) {
    MutexData * d = (MutexData*) data;
    for (int i = 0 ; i < 500 ; i++ ) {
        d->mutex.lock();
        d->Inc();
        d->mutex.unlock();
    }
    task->Done();
}


void inc_a(ThreadTask *task, void * data) {
    MutexData * d = (MutexData*) data;
    std::unique_lock<std::mutex> mlock(d->mutex);
    d->Inc();
    task->Done();
}

void set_a_1(ThreadTask *task, void * data) { 
    int * a = (int*) data;
    *a = 1;
    task->Done();
}

TEST(ThreadPoolTest, ThreadTaskWait) {
    ThreadPool pool(4);
    int a = 0;
    ThreadTask task(set_a_1, &a);
    pool.AddTask(&task);
    task.Wait();
    EXPECT_EQ(a, 1);
}

TEST(ThreadPoolTest, Thread2TaskWait) {
    ThreadPool pool(2);
    int a = 0;
    ThreadTask task1(set_a_1, &a);
    ThreadTask task2(set_a_1, &a);
    pool.AddTask(&task1);
    pool.AddTask(&task2);
    task1.Wait();
    task2.Wait();
    EXPECT_EQ(a, 1);
}


TEST(ThreadPoolTest, Thread2TaskMutexWait) {
    ThreadPool pool(2);
    int a = 0;
    MutexData data(&a);
    ThreadTask task1(inc_a, &data);
    ThreadTask task2(inc_a, &data);
    pool.AddTask(&task1);
    pool.AddTask(&task2);
    task1.Wait();
    task2.Wait();
    EXPECT_EQ(a, 2);
}




TEST(ThreadPoolTest, Thread2TaskMutexRace) {
    ThreadPool pool(6);

    int a = 0;
    MutexData data(&a);

    std::vector<ThreadTask*> tasks;
    for (int i = 0 ; i < 10 ; i++ ) {
        auto task = new ThreadTask(inc_a_500, &data);
        tasks.push_back(task);
        pool.AddTask(task);
    }
    for (std::vector<ThreadTask*>::iterator it = tasks.begin(); it != tasks.end() ; it++ ) {
        (*it)->Wait();
    }
    EXPECT_EQ(a, 500 * 10);
}


TEST(ThreadPoolTest, ThreadMoreTasks) {
    ThreadPool pool(2);
    int a = 0;
    MutexData data(&a);
    ThreadTask task1(inc_a, &data);
    ThreadTask task2(inc_a, &data);
    ThreadTask task3(inc_a, &data);
    ThreadTask task4(inc_a, &data);
    pool.AddTask(&task1);
    pool.AddTask(&task2);
    pool.AddTask(&task3);
    pool.AddTask(&task4);
    task1.Wait();
    task2.Wait();
    task3.Wait();
    task4.Wait();
    EXPECT_EQ(a, 4);
}






TEST(ThreadPoolTest, TestGlobalThread) {
    int a = 0;
    ThreadTask parent(parent_a, &a);
    global_pool.AddTask(&parent);
    parent.Wait();
    EXPECT_EQ(a, 3); // parent + 2 sub tasks
}





int main(int argc, char **argv) {
    // GoogleProfilerAuto profiler(argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
