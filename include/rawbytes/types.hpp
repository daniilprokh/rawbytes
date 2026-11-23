#pragma once

#include "rawbytes.hpp"

#include <cstddef>
#include <concepts>

#ifndef RAWBYTES_SIZE_TYPE
#define RAWBYTES_SIZE_TYPE std::size_t
#endif

static_assert(std::integral<RAWBYTES_SIZE_TYPE>);

RAWBYTES_NAMESPACE_BEGIN

using Byte = unsigned char;
using Char = char;

using Size = RAWBYTES_SIZE_TYPE;
using Pos = Size;
using Diff = std::ptrdiff_t;

using NullPtr = std::nullptr_t;

RAWBYTES_NAMESPACE_END
