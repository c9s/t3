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

struct MutexData { 
    int * a;
    std::mutex mutex;
    MutexData(int * a) : a(a) { }

    void Inc() {
        std::lock_guard<std::mutex> lock(mutex);
        *a = *a + 1;
    }
};

ThreadPool global_pool(4);

TEST(ThreadPool, TestThreadConstructorAndDestructor) {
    ThreadPool pool(4);
}

void inc_a(ThreadTask *task, void * data);
void inc_a_500(ThreadTask *task, void * data);
void parent_a(ThreadTask *task, void * data);


void sub_a(ThreadTask *task, void * data) {
    /*
    std::unique_lock<std::mutex> lock(task->mutex);
    */
    MutexData * d = (MutexData*) data;
    d->Inc();

    /*
    task->done_ = true;
    lock.unlock();
    task->cond.notify_all();
    */
}

void parent_a(ThreadTask *task, void * data) {
    MutexData * tdata = (MutexData*) data;

    ThreadTask task1(sub_a, tdata);
    ThreadTask task2(sub_a, tdata);

    task->pool()->AddTask(&task1);
    task->pool()->AddTask(&task2);

    std::cout << "Waiting sub 2" << std::endl;
    /*
    std::unique_lock<std::mutex> lock2(task2.mutex);
    while (!task2.done_) task2.cond.wait(lock2);
    lock2.unlock();
    task2.cond.notify_one();
    */
    task2.Wait();
    std::cout << "Done sub 2" << std::endl;




    std::cout << "Waiting sub 1" << std::endl;
    /*
    std::unique_lock<std::mutex> lock1(task1.mutex);
    while (!task1.done_) task1.cond.wait(lock1);
    lock1.unlock();
    task1.cond.notify_one();
    */
    task1.Wait();
    std::cout << "Done sub 1" << std::endl;
}

void inc_a_500(ThreadTask *task, void * data) {
    MutexData * d = (MutexData*) data;
    for (int i = 0 ; i < 500 ; i++ ) {
        d->Inc();
    }
}


void inc_a(ThreadTask *task, void * data) {
    MutexData * d = (MutexData*) data;
    d->Inc();
    // task->Done();
}

void set_a_1(ThreadTask *task, void * data) { 
    int * a = (int*) data;
    *a = 1;
    // task->Done();
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
    ThreadPool pool(3);

    int a = 0;
    MutexData data(&a);

    std::vector<ThreadTask*> tasks;
    for (int i = 0 ; i < 100 ; i++ ) {
        auto task = new ThreadTask(inc_a_500, &data);
        tasks.push_back(task);
        pool.AddTask(task);
    }
    for (std::vector<ThreadTask*>::iterator it = tasks.begin(); it != tasks.end() ; it++ ) {
        (*it)->Wait();
    }
    EXPECT_EQ(a, 500 * 100);
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






TEST(ThreadPoolTest, TestParentThreadTask) {
    ThreadPool pool(4);

    int a = 0;
    for (int x = 0; x < 1000 ; x++ ) {
        MutexData data(&a);
        ThreadTask parent1(parent_a, &data);

        std::cout << "===> Start parent " << x << std::endl;
        pool.AddTask(&parent1); // it's not locking the done mutex
        std::cout << "===> Waiting parent 1" << x << std::endl;
        parent1.Wait();
        std::cout << "===> Done parent 1" << x << std::endl;

        /*
        std::unique_lock<std::mutex> lock1(parent1.mutex);
        while (!parent1.done_) parent1.cond.wait(lock1);
        std::cout << "Done parent 1" << std::endl;
        lock1.unlock();
        parent1.cond.notify_one();
        */

    }
}





int main(int argc, char **argv) {
    // GoogleProfilerAuto profiler(argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
