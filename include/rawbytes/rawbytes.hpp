#pragma once

#ifndef RAWBYTES_NAMESPACE
#define RAWBYTES_NAMESPACE rawbytes
#endif

#define RAWBYTES_NAMESPACE_BEGIN namespace RAWBYTES_NAMESPACE {
#define RAWBYTES_NAMESPACE_END }

#ifndef RAWBYTES_ASSERT
#include <cassert>
#define RAWBYTES_ASSERT(exp, msg) assert((void(msg), exp))
#endif

#define RAWBYTES_ASSERT_TRUE(exp, msg) RAWBYTES_ASSERT(exp, msg)
#define RAWBYTES_ASSERT_FALSE(exp, msg) RAWBYTES_ASSERT(!exp, msg)

#define RAWBYTES_ASSERT_EQ(lhs, rhs, msg) RAWBYTES_ASSERT(lhs == rhs, msg)
#define RAWBYTES_ASSERT_NE(lhs, rhs, msg) RAWBYTES_ASSERT(lhs != rhs, msg)
#define RAWBYTES_ASSERT_LT(lhs, rhs, msg) RAWBYTES_ASSERT(lhs < rhs, msg)
#define RAWBYTES_ASSERT_LE(lhs, rhs, msg) RAWBYTES_ASSERT(lhs <= rhs, msg)
#define RAWBYTES_ASSERT_GT(lhs, rhs, msg) RAWBYTES_ASSERT(lhs > rhs, msg)
#define RAWBYTES_ASSERT_GE(lhs, rhs, msg) RAWBYTES_ASSERT(lhs >= rhs, msg)
