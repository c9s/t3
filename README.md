t3 - Lightweight Thread Pool for C++
======================================


## ThreadPool

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

## Channel

This implements a Go channel like structure for piping data to another thread.


Producer Thread:

```cc
t3::Channel<int> channel;
for (int i = 0 ; i < 10 ; i++ ) {
    channel.push(i);
}
```

Consumer Thread:

```cc
int a;
while ( a = channel.pop() ) {
    // consume
}
```

## Build

    GTEST_ROOT=$(pwd)/gtest-1.7.0 cmake .
    make test
