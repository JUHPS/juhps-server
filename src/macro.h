#ifndef __SRC_MACRO_H__
#define __SRC_MACRO_H__

#include <string.h>
#include <assert.h>
#include "log.h"
#include "util.h"

#if defined __GNUC__ || defined __llvm__
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#   define JUJIMEIZUO_LIKELY(x)       __builtin_expect(!!(x), 1)
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#   define JUJIMEIZUO_UNLIKELY(x)     __builtin_expect(!!(x), 0)
#else
#   define JUJIMEIZUO_LIKELY(x)      (x)
#   define JUJIMEIZUO_UNLIKELY(x)      (x)
#endif

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