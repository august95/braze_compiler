//
// pch.h
//

#pragma once

#include "gtest/gtest.h"


#define STRINGS_EQUAL(str, str2) \
    (str && str2 && (strcmp(str, str2) == 0))

