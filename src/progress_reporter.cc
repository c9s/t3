/*
 * progress_reporter.cc
 * Copyright (C) 2014 c9s <c9s@c9smba.local>
 *
 * Distributed under terms of the MIT license.
 */
#include "progress_reporter.h"


/**
 * bool displayHandler(float finished, float total) {
 *
 *      return false; // stop the progress reporter.
 * }
 *
 *
 * ProgressReporter reporter(&task, displayHandler);
 *
 * the progress reporter watches & waits on the mutex of the task and return
 * the progress value to the display handler
 */


