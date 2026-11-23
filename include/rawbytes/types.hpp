#pragma once

#include "rawbytes.hpp"

#include <cstddef>
#include <concepts>

#ifndef RAWBYTES_SIZE_TYPE
#define RAWBYTES_SIZE_TYPE std::size_t
#endif

#ifndef RAWBYTES_DIFF_TYPE
#define RAWBYTES_DIFF_TYPE std::ptrdiff_t
#endif

static_assert(std::integral<RAWBYTES_SIZE_TYPE>);
static_assert(std::integral<RAWBYTES_DIFF_TYPE>);

RAWBYTES_NAMESPACE_BEGIN

using Byte = unsigned char;
using Char = char;

using Size = RAWBYTES_SIZE_TYPE;
using Pos = Size;
using Diff = RAWBYTES_DIFF_TYPE;

using NullPtr = std::nullptr_t;

RAWBYTES_NAMESPACE_END
