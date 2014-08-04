enable_testing()
find_package(GTest REQUIRED)
set(PROJECT_TEST_NAME "${PROJECT_NAME}_test")
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(thread_pool_test thread_pool_test.cc)
target_link_libraries(thread_pool_test t3 ${GTEST_BOTH_LIBRARIES})
add_test(thread_pool_test thread_pool_test)

add_executable(wait_group_test wait_group_test.cc)
target_link_libraries(wait_group_test t3 ${GTEST_BOTH_LIBRARIES})
add_test(wait_group_test wait_group_test)
