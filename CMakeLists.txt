# cmake file examples
# https://code.google.com/p/opencv-feature-tracker/source/browse/CMakeLists.txt?r=f804b03e704147e65183c19a50f57abedb22f45c
cmake_minimum_required(VERSION 2.8)
project(t3)

# -DCMAKE_BUILD_TYPE=DEBUG
# -DCMAKE_BUILD_TYPE=RELEASE
# if(NOT CMAKE_BUILD_TYPE )
#     set( CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
# endif()

SET(CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/cmake_modules
    ${CMAKE_MODULE_PATH}
    )

link_directories(${LINK_DIRECTORIES} /opt/local/lib /usr/lib)
include_directories(. ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src /opt/local/include /usr/include)
# set (CMAKE_CXX_FLAGS "-std=c++0x -arch x86_64 -stdlib=libc++ -g3 -Wall -O0")

# debug
# -pg for profile
# set (CMAKE_CXX_FLAGS "-std=c++0x -arch x86_64 -stdlib=libc++ -Wall -Wno-long-long -Wno-unused-value -Wno-overloaded-virtual -O3")
# set (CMAKE_CXX_FLAGS "-std=c++0x -arch x86_64 -stdlib=libc++ -g3 -Wall -Wno-long-long -Wno-unused-value -Wno-overloaded-virtual -O0")

# http://www.openguru.com/2009/04/cmake-detecting-platformoperating.html
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "-std=c++0x -arch x86_64 -stdlib=libc++ -Wall -Wno-long-long -Wno-unused-value -Wno-overloaded-virtual")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-long-long -Wno-unused-value -Wno-overloaded-virtual")
endif()

if(${CMAKE_BUILD_TYPE} MATCHES "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
# elseif(${CMAKE_BUILD_TYPE} MATCHES "Debug")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -O0 -g3")
endif()

# set(gtest_SOURCE_DIR gtest-1.7.0)
# add_subdirectory(${gtest_SOURCE_DIR})
# include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_subdirectory(src)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    enable_testing()
    set(ENV{GTEST_ROOT} ${PROJECT_SOURCE_DIR}/gtest-1.7.0)
    add_subdirectory(gtest-1.7.0)
    add_subdirectory(tests)
endif()

