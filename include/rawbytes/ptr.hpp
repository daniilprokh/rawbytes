#pragma once

#include "cast.hpp"

#include <utility>

RAWBYTES_NAMESPACE_BEGIN

template <Raw T>
class Ptr
{
public:
    Ptr() noexcept = default;
    ~Ptr() noexcept = default;

    Ptr(const Ptr&) noexcept = default;
    Ptr& operator=(const Ptr&) noexcept = default;

    Ptr(Ptr&& oth) noexcept
        : ptr_(std::exchange(oth.ptr_, nullptr))
    {}
    Ptr& operator=(Ptr&& oth) noexcept
    {
        if (&oth != this) {
            ptr_ = std::exchange(oth.ptr_, nullptr);
        }
        return *this;
    }

    Ptr([[maybe_unused]] NullPtr) noexcept
    {}

    explicit Ptr(T* ptr) noexcept
        : ptr_(ptr)
    {}

    template <Raw U>
    requires traits::ConstImplicationWith<U, T>
    explicit Ptr(U* ptr) noexcept
       : ptr_(Cast<T, traits::CopyConstType<T, U>>(ptr))
    {}

    template <Raw U>
    requires traits::ConstImplicationWith<U, T>
    explicit Ptr(Ptr<U> oth) noexcept
        : ptr_(oth.template Get<T>())
    {}

    template <Raw U>
    requires traits::ConstImplicationWith<U, T>
    Ptr& operator=(Ptr<U> oth) noexcept
    {
        ptr_ = oth.template Get<T>();
        return *this;
    }

    bool operator==([[maybe_unused]] NullPtr) const noexcept
    {
        return ptr_ == nullptr;
    }

    bool operator==(const Ptr&) const noexcept = default;

    template <Raw U>
    bool operator==(Ptr<U> oth) const noexcept
    {
        return ptr_ == oth.template Get<traits::CopyConstType<U, T>>();
    }

    Ptr& operator++() noexcept
    {
        if (ptr_ != nullptr) {
            ++ptr_;
        }
        return *this;
    }
    Ptr& operator--() noexcept
    {
        if (ptr_ != nullptr) {
            --ptr_;
        }
        return *this;
    }

    Ptr operator++(int) noexcept
    {
        Ptr raw_ptr{ *this };
        if (ptr_ != nullptr) {
            ++ptr_;
        }
        return raw_ptr;
    }
    Ptr operator--(int) noexcept
    {
        Ptr raw_ptr{ *this };
        if (ptr_ != nullptr) {
            --ptr_;
        }
        return raw_ptr;
    }

    Ptr& operator+=(Diff diff) noexcept
    {
        if (ptr_ != nullptr) {
            ptr_ += diff;
        }
        return *this;
    }
    Ptr& operator-=(Diff diff) noexcept
    {
        if (ptr_ != nullptr) {
            ptr_ -= diff;
        }
        return *this;
    }

    Ptr operator+(Diff diff) const noexcept
    {
        return ptr_ ? Ptr{ ptr_ + diff } : Ptr{};
    }
    
    Ptr operator-(Diff diff) const noexcept
    {
        return ptr_ ? Ptr{ ptr_ - diff } : Ptr{};
    }

    T* operator->() const noexcept
    {
        return ptr_;
    }

    void Reset([[maybe_unused]] NullPtr = nullptr) noexcept
    {
        ptr_ = nullptr;
    }

    void Reset(T* ptr)
    {
        ptr_ = ptr;
    }

    template <Raw U>
    requires traits::ConstImplicationWith<U, T>
    void Reset(U* ptr) noexcept
    {
        ptr_ = Cast<T, traits::CopyConstType<T, U>>(ptr);
    }

    template <Raw U = T>
    requires traits::ConstImplicationWith<T, U>
    U* Get() const noexcept
    {
        return Cast<U, traits::CopyConstType<U, T>>(ptr_);
    }

    traits::CopyConstType<T, Byte>* GetByte() const noexcept
    {
        return CastToByte(ptr_);
    }

    traits::CopyConstType<T, Char>* GetChar() const noexcept
    {
        return CastToChar(ptr_);
    }

    Size GetSizeBytes() const noexcept
    {
        return ptr_ != nullptr ? sizeof(T) : 0;
    }
private:
    T* ptr_{ nullptr };
};

template <Raw T>
inline Diff operator-(Ptr<T> lhs, Ptr<T> rhs) noexcept
{
    return lhs.Get() - rhs.Get();
}

RAWBYTES_NAMESPACE_END
