Build
==========


```cc
ThreadPool pool(2);

void parent_a(t3::ThreadTask *task, MutexData * data) {


}

int a = 0;
for (int x = 0; x < 1000 ; x++ ) {
    MutexData data(&a);
    ThreadTask parent1(parent_a, &data);

    pool.AddTask(&parent1); // it's not locking the done mutex
    parent1.Wait();
}
```

## Build

    GTEST_ROOT=$(pwd)/gtest-1.7.0 cmake .
    make test
