add_library(thread_pool thread_pool.cc thread_task.cc)
target_link_libraries(thread_pool pthread)
