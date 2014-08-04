add_library(t3 thread_pool.cc thread_task.cc channel.cc wait_group.cc)
target_link_libraries(t3 pthread)
