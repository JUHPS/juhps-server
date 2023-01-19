#ifndef __SRC_MACRO_H__
#define __SRC_MACRO_H__

#include <string.h>
#include <assert.h>
#include "log.h"
#include "util.h"


// 断言宏封装
#define JUJIMEIZUO_ASSERT(x) \
    if (!(x)) { \
        JUJIMEIZUO_LOG_ERROR(JUJIMEIZUO_LOG_ROOT()) << "ASSERTION: " #x \
            << "\nbacktrace:\n" \
            << jujimeizuo::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

// 断言宏封装
#define JUJIMEIZUO_ASSERT_E(x, w) \
    if (!(x)) { \
        JUJIMEIZUO_LOG_ERROR(JUJIMEIZUO_LOG_ROOT()) << "ASSERTION: " #x \
            << "\n" << w \
            << "\nbacktrace:\n" \
            << jujimeizuo::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#endif