#pragma once

#include "rawbytes.hpp"

#include <concepts>
#include <type_traits>

RAWBYTES_NAMESPACE_BEGIN

namespace traits {

// Const

template <class T>
concept Const = std::is_const_v<T>;

template <class T>
concept NonConst = !Const<T>;

// ConstConsistent

template <bool IsConst, class T>
concept ConstConsistent = (IsConst && Const<T>) || (!IsConst && NonConst<T>);

template <class T, class U>
concept ConstConsistentWith = (Const<T> && Const<U>) || (NonConst<T> && NonConst<U>);

// ConstImplication

template <bool IsAntecedentConst, class Consequent>
concept ConstImplication = !IsAntecedentConst || Const<Consequent>; 

template <class Antecedent, class Consequent>
concept ConstImplicationWith = NonConst<Antecedent> || Const<Consequent>;

// ConditionalConst

template <bool Condition, class T>
struct ConditionalConst : std::conditional<Condition, std::add_const_t<T>, T>
{};

template <bool Condition, class T>
using ConditionalConstType = typename ConditionalConst<Condition, T>::type;

// CopyConst

template <class From, class To>
struct CopyConst : ConditionalConst<std::is_const_v<From>, To>
{};

template <class T>
struct CopyConst<T, T>
{
    using type = T;
};

template <class From, class To>
using CopyConstType = typename CopyConst<From, To>::type;

} // namespace traits

template <class T>
concept Raw = !std::is_array_v<T>
              && std::is_standard_layout_v<T>
              && std::is_trivially_copyable_v<T>
              && alignof(T) == 1;

template <class T>
concept RawByte = Raw<T> && sizeof(T) == 1;

RAWBYTES_NAMESPACE_END
