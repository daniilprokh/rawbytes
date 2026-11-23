#pragma once

#include "types.hpp"
#include "traits.hpp"

RAWBYTES_NAMESPACE_BEGIN

template <Raw To, Raw From>
requires traits::ConstConsistentWith<To, From>
To* Cast(From* from) noexcept
{
    return reinterpret_cast<To*>(from);
}

template <Raw To>
To* CastByte(Byte* from) noexcept
{
    return Cast<To>(from);
}

template <Raw To>
const To* CastByte(const Byte* from) noexcept
{
    return Cast<const To>(from);
}

template <Raw To>
To* CastChar(Char* from) noexcept
{
    return Cast<To>(from);
}

template <Raw To>
const To* CastChar(const Char* from) noexcept
{
    return Cast<const To>(from);
}

template <Raw From>
Byte* CastToByte(From* from) noexcept
{
    return Cast<Byte>(from);
}

template <Raw From>
const Byte* CastToByte(const From* from) noexcept
{
    return Cast<const Byte>(from);
}

template <Raw From>
Char* CastToChar(From* from) noexcept
{
    return Cast<Char>(from);
}

template <Raw From>
const Char* CastToChar(const From* from) noexcept
{
    return Cast<const Char>(from);
}

RAWBYTES_NAMESPACE_END
