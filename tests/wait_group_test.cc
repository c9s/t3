/*
 * wait_group_test.cc
 * Copyright (C) 2014 c9s <c9s@c9smba.local>
 *
 * Distributed under terms of the MIT license.
 */

#include "wait_group.h"
#include <gperftools/profiler.h>
#include "gtest/gtest.h"

using namespace t3;

TEST(WaitGroupTest, WaitAndDone) {
    WaitGroup wg(3);
    wg.Inc();
    wg.Done();
    // EXPECT_EQ(wg.Finished())
}

int main(int argc, char **argv) {
    // GoogleProfilerAuto profiler(argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



