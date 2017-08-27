#include "common.h"

#include <cstdlib>

using namespace common;

float common::rangeRand(float min, float max) {
    return rand() % (int)(max - min) + min;
}

