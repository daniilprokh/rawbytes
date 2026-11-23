#pragma once

#include "ptr.hpp"

#include <cstring>

#ifdef RAWBYTES_ENABLE_STD_SPAN 
#include <span>
#endif
#ifdef RAWBYTES_ENABLE_STD_STRING_VIEW
#include <string_view>
#endif

RAWBYTES_NAMESPACE_BEGIN

template <RawByte T>
class Slice
{
public:
    Slice() = default;
    ~Slice() = default;

    Slice(const Slice&) = default;
    Slice& operator=(const Slice&) = default;

    Slice(Slice&& oth) noexcept
        : ptr_(std::move(oth.ptr_)),
          size_(std::exchange(oth.size_, 0))
    {}
    Slice& operator=(Slice&& oth) noexcept
    {
        if (&oth != this) {
            ptr_ = std::move(oth.ptr_);
            size_ = std::exchange(oth.size_, 0);
        }
        return *this;
    }

    Slice([[maybe_unused]] NullPtr, [[maybe_unused]] Size)
    {}

    Slice(T* data, Size size)
        : ptr_(data),
          size_(data != nullptr ? size : 0)
    {
        if constexpr (std::is_signed_v<Size>) {
            if (size_ < 0) {
                size_ = 0;
            }
        }
    }

    template<RawByte U>
    requires traits::ConstImplicationWith<U, T>
    Slice(U* data, Size size)
        : ptr_(data),
          size_(data != nullptr ? size : 0)
    {
        if constexpr (std::is_signed_v<Size>) {
            if (size_ < 0) {
                size_ = 0;
            }
        }
    }

    Slice(T* begin, T* end)
        : Slice(begin, 
                static_cast<Size>(static_cast<Diff>(end - begin) > 0
                                  ? end - begin
                                  : 0))
    {}

    template<RawByte U>
    requires traits::ConstImplicationWith<U, T>
    Slice(U* begin, U* end)
        : Slice(begin,
                static_cast<Size>(static_cast<Diff>(end - begin) > 0
                                  ? end - begin
                                  : 0))
    {}

    template <Raw R>
    requires traits::ConstImplicationWith<R, T>
    explicit Slice(R* obj)
        : ptr_(obj),
          size_(obj != nullptr ? sizeof(*obj) : 0)
    {}

    template <RawByte U>
    requires traits::ConstImplicationWith<U, T>
    explicit Slice(Ptr<U> data, Size size)
        : Slice(data.Get(), size)
    {}

    template <Raw R>
    requires traits::ConstImplicationWith<R, T>
    explicit Slice(Ptr<R> obj)
        : ptr_(obj),
          size_(obj.GetSizeBytes())
    {}

#ifdef RAWBYTES_ENABLE_STD_SPAN
    explicit Slice(std::span<T> span)
        : Slice(span.data(), static_cast<Size>(span.size()))
    {}

    template<RawByte U>
    requires traits::ConstImplicationWith<U, T>
    explicit Slice(std::span<U> span)
        : Slice(span.data(), static_cast<Size>(span.size()))
    {}
#endif

#ifdef RAWBYTES_ENABLE_STD_STRING_VIEW
    template<RawByte U>
    requires traits::Const<T> && std::is_trivially_default_constructible_v<U>
    explicit Slice(std::basic_string_view<U> view) 
        : Slice(view.data(), static_cast<Size>(view.size()))
    {}
#endif

    [[nodiscard]] bool IsEmpty() const
    {
        return size_ == 0;
    }

    [[nodiscard]] bool HasSize(Size size) const
    {
        if constexpr (std::is_signed_v<Size>) {
            if (size < 0) {
                return false;
            }
        }
        return size_ >= size;
    }

    // Remove

    void RemoveLeft(Size size)
    {
        if (size == 0) [[unlikely]] {
            return;
        }
        RAWBYTES_ASSERT_TRUE(HasSize(size), "SAMPLE TEXT");
        RAWBYTES_ASSERT_NE(ptr_.Get(), nullptr, "SAMPLE TEXT");
        size_ -= size;
        if (size_ == 0) {
            ptr_.Reset();
        } else {
            ptr_ += size;
        }
    }
    [[nodiscard]] bool RemoveLeftCheck(Size size)
    {
        if (!HasSize(size)) {
            return false;
        }
        RemoveLeft(size);
        return true;
    }

    void RemoveRight(Size size)
    {
        if (size == 0) [[unlikely]] {
            return;
        }
        RAWBYTES_ASSERT_TRUE(HasSize(size), "SAMPLE TEXT");
        RAWBYTES_ASSERT_NE(ptr_.Get(), nullptr, "SAMPLE TEXT");
        size_ -= size;
        if (size_ == 0) {
            ptr_.Reset();
        }
    }
    [[nodiscard]] bool RemoveRightCheck(Size size)
    {
        if (!HasSize(size)) {
            return false;
        }
        RemoveRight(size);
        return true;
    }

    void RemoveAll()
    {
        ptr_.Reset();
        size_ = 0;
    }

    // Peek

    template <RawByte U>
    requires traits::ConstImplicationWith<T, U>
    void PeekLeft(Size size, Slice<U>& slice) const
    {
        PeekMid(0, size, slice);
    }
    template <RawByte U>
    requires traits::ConstImplicationWith<T, U>
    [[nodiscard]] bool PeekLeftCheck(Size size, Slice<U>& slice) const
    {
        return PeekMidCheck(0, size, slice);
    }

    template <RawByte U>
    requires traits::ConstImplicationWith<T, U>
    void PeekRight(Size size, Slice<U>& slice) const
    {
        RAWBYTES_ASSERT_TRUE(HasSize(size), "SAMPLE TEXT");
        PeekMid(size_ - size, size, slice);
    }
    template <RawByte U>
    requires traits::ConstImplicationWith<T, U>
    [[nodiscard]] bool PeekRightCheck(Size size, Slice<U>& slice) const
    {
        if (!HasSize(size)) {
            return false;
        }
        return PeekMidCheck(size_ - size, size, slice);
    }

    template <RawByte U>
    requires traits::ConstImplicationWith<T, U>
    void PeekMid(Pos pos, Size size, Slice<U>& slice) const
    {
        RAWBYTES_ASSERT_LT(pos, size_, "SAMPLE TEXT");
        RAWBYTES_ASSERT_TRUE(HasSize(size), "SAMPLE TEXT");
        RAWBYTES_ASSERT_TRUE(HasSize(pos + size), "SAMPLE TEXT");
        RAWBYTES_ASSERT_NE(ptr_.Get(), nullptr, "SAMPLE TEXT");
        slice = Slice<U>(ptr_ + pos, size);
    }
    template <RawByte U>
    requires traits::ConstImplicationWith<T, U>
    [[nodiscard]] bool PeekMidCheck(Pos pos, Size size, Slice<U>& slice) const
    {
        if (pos >= size_) {
            return false;
        }
        if (!HasSize(size)) {
            return false;
        }
        if (!HasSize(pos + size)) {
            return false;
        }
        PeekMid(pos, size, slice);
        return true;
    }

    // Take

    template <RawByte U>
    requires traits::ConstImplicationWith<T, U>
    void TakeLeft(Size size, Slice<U>& slice)
    {
        PeekLeft(size, slice);
        RemoveLeft(size);
    }
    template <RawByte U>
    requires traits::ConstImplicationWith<T, U>
    [[nodiscard]] bool TakeLeftCheck(Size size, Slice<U>& slice)
    {
        if (!PeekLeftCheck(size, slice)) {
            return false;
        }
        RemoveLeft(size);
        return true;
    }

    template <RawByte U>
    requires traits::ConstImplicationWith<T, U>
    void TakeRight(Size size, Slice<U>& slice)
    {
        PeekRight(size, slice);
        RemoveRight(size);
    }
    template <RawByte U>
    requires traits::ConstImplicationWith<T, U>
    [[nodiscard]] bool TakeRightCheck(Size size, Slice<U>& slice)
    {
        if (!PeekRightCheck(size, slice)) {
            return false;
        }
        RemoveRight(size);
        return true;
    }

    // CopyTo

    template <RawByte U>
    requires traits::NonConst<U>
    void CopyToLeft(U* dst, Size size) const
    {
        CopyToMid(0, dst, size);
    }
    template <RawByte U>
    requires traits::NonConst<U>
    [[nodiscard]] bool CopyToLeftCheck(U* dst, Size size) const
    {
        return CopyToMidCheck(0, dst, size);
    }

    template <RawByte U>
    requires traits::NonConst<U>
    void CopyToRight(U* dst, Size size) const
    {
        RAWBYTES_ASSERT_TRUE(HasSize(size), "SAMPLE TEXT");
        CopyToMid(size_ - size, dst, size);
    }
    template <RawByte U>
    requires traits::NonConst<U>
    [[nodiscard]] bool CopyToRightCheck(U* dst, Size size) const
    {
        if (!HasSize(size)) {
            return false;
        }
        return CopyToMidCheck(size_ - size, dst, size);
    }

    template <RawByte U>
    requires traits::NonConst<U>
    void CopyToMid(Pos pos, U* dst, Size size) const
    {
        RAWBYTES_ASSERT_NE(dst, nullptr, "SAMPLE TEXT");
        RAWBYTES_ASSERT_LT(pos, size_, "SAMPLE TEXT");
        RAWBYTES_ASSERT_TRUE(HasSize(size), "SAMPLE TEXT");
        RAWBYTES_ASSERT_TRUE(HasSize(pos + size), "SAMPLE TEXT");
        RAWBYTES_ASSERT_NE(ptr_.Get(), nullptr, "SAMPLE TEXT");
        std::memcpy(dst, ptr_.GetByte() + pos, size);
    }

    template <RawByte U>
    requires traits::NonConst<U>
    [[nodiscard]] bool CopyToMidCheck(Pos pos, U* dst, Size size) const
    {
        if (dst == nullptr) {
            return false;
        }
        if (pos >= size_) {
            return false;
        }
        if (!HasSize(size)) {
            return false;
        }
        if (!HasSize(pos + size)) {
            return false;
        }
        CopyToMid(pos, dst, size);
        return true;
    }

    template <Raw R>
    requires traits::NonConst<R>
    void CopyToLeft(R* to) const
    {
        CopyToMid(0, to);
    }
    template <Raw R>
    requires traits::NonConst<R>
    [[nodiscard]] bool CopyToLeftCheck(R* to) const
    {
        return CopyToMidCheck(0, to);
    }

    template <Raw R>
    requires traits::NonConst<R>
    void CopyToRight(R* to) const
    {
        RAWBYTES_ASSERT_TRUE(HasSize(sizeof(R)), "SAMPLE TEXT");
        CopyToMid(size_ - sizeof(R), to);
    }
    template <Raw R>
    requires traits::NonConst<R>
    [[nodiscard]] bool CopyToRightCheck(R* to) const
    {
        if (!HasSize(sizeof(R))) {
            return false;
        }
        return CopyToMidCheck(size_ - sizeof(R), to);
    }

    template <Raw R>
    requires traits::NonConst<R>
    void CopyToMid(Pos pos, R* to) const
    {
        RAWBYTES_ASSERT_NE(to, nullptr, "SAMPLE TEXT");
        RAWBYTES_ASSERT_LT(pos, size_, "SAMPLE TEXT");
        RAWBYTES_ASSERT_TRUE(HasSize(sizeof(R)), "SAMPLE TEXT");
        RAWBYTES_ASSERT_TRUE(HasSize(pos + sizeof(R)), "SAMPLE TEXT");
        RAWBYTES_ASSERT_NE(ptr_.Get(), nullptr, "SAMPLE TEXT");
        std::memcpy(to, ptr_.GetByte() + pos, sizeof(R));
    }
    template <Raw R>
    requires traits::NonConst<R>
    [[nodiscard]] bool CopyToMidCheck(Pos pos, R* to) const
    {
        if (to == nullptr) {
            return false;
        }
        if (pos >= size_) {
            return false;
        }
        if (!HasSize(sizeof(R))) {
            return false;
        }
        if (!HasSize(pos + sizeof(R))) {
            return false;
        }
        CopyToMid(pos, to);
        return true;
    }

    // CopyToAndRemove

    template <RawByte U>
    requires traits::NonConst<U>
    void CopyToAndRemoveLeft(U* dst, Size size) const
    {
        CopyToLeft(dst, size);
        RemoveLeft(size);
    }

    template <RawByte U>
    requires traits::NonConst<U>
    [[nodiscard]] bool CopyToAndRemoveLeftCheck(U* dst, Size size) const
    {
        if (!CopyToLeftCheck(dst, size)) {
            return false;
        }
        RemoveLeft(size);
        return true;
    }

    template <RawByte U>
    requires traits::NonConst<U>
    void CopyToAndRemoveRight(U* dst, Size size) const
    {
        CopyToRight(dst, size);
        RemoveRight(size);
    }

    template <RawByte U>
    requires traits::NonConst<U>
    [[nodiscard]] bool CopyToAndRemoveRightCheck(U* dst, Size size) const
    {
        if (!CopyToRightCheck(dst, size)) {
            return false;
        }
        RemoveRight(size);
        return true;
    }

    template <Raw R>
    requires traits::NonConst<R>
    void CopyToAndRemoveLeft(R* dst)
    {
        CopyToLeft(dst);
        RemoveLeft(sizeof(R));
    }
    template <Raw R>
    requires traits::NonConst<R>
    [[nodiscard]] bool CopyToAndRemoveLeftCheck(R* dst)
    {
        if (!CopyToLeftCheck(dst)) {
            return false;
        }
        RemoveLeft(sizeof(R));
        return true;
    }

    template <Raw R>
    requires traits::NonConst<R>
    void CopyToAndRemoveRight(R* to)
    {
        CopyToRight(to);
        RemoveRight(sizeof(R));
    }
    template <Raw R>
    requires traits::NonConst<R>
    [[nodiscard]] bool CopyToAndRemoveRightCheck(R* to)
    {
        if (!CopyToRightCheck(to)) {
            return false;
        }
        RemoveRight(sizeof(R));
        return true;
    }

    // Cast

    template <Raw R>
    requires traits::ConstImplicationWith<T, R>
    void CastLeft(Ptr<R>& ptr) const
    {
        CastMid(0, ptr);
    }
    template <Raw R>
    requires traits::ConstImplicationWith<T, R>
    [[nodiscard]] bool CastLeftCheck(Ptr<R>& ptr) const
    {
        return CastMidCheck(0, ptr);
    }

    template <Raw R>
    requires traits::ConstImplicationWith<T, R>
    void CastRight(Ptr<R>& ptr) const
    {
        RAWBYTES_ASSERT_TRUE(HasSize(sizeof(R)), "SAMPLE TEXT");
        CastMid(size_ - sizeof(R), ptr);
    }
    template <Raw R>
    requires traits::ConstImplicationWith<T, R>
    [[nodiscard]] bool CastRightCheck(Ptr<R>& ptr) const
    {
        if (!HasSize(sizeof(R))) {
            return false;
        }
        return CastMidCheck(ptr);
    }

    template <Raw R>
    requires traits::ConstImplicationWith<T, R>
    void CastMid(Pos pos, Ptr<R>& ptr) const
    {
        RAWBYTES_ASSERT_LT(pos, size_, "SAMPLE TEXT");
        RAWBYTES_ASSERT_TRUE(HasSize(sizeof(R)), "SAMPLE TEXT");
        RAWBYTES_ASSERT_TRUE(HasSize(pos + sizeof(R)), "SAMPLE TEXT");
        RAWBYTES_ASSERT_NE(ptr_.Get(), nullptr, "SAMPLE TEXT");
        ptr = ptr_ + pos;
    }
    template <Raw R>
    requires traits::ConstImplicationWith<T, R>
    [[nodiscard]] bool CastMidCheck(Pos pos, Ptr<R>& ptr) const
    {
        if (pos >= size_) {
            return false;
        }
        if (!HasSize(sizeof(R))) {
            return false;
        }
        if (!HasSize(pos + sizeof(R))) {
            return false;
        }
        CastMid(pos, ptr);
        return true;
    }

    // CastAndRemove

    template <Raw R>
    requires traits::ConstImplicationWith<T, R>
    void CastAndRemoveLeft(Ptr<R>& ptr)
    {
        CastLeft(ptr);
        RemoveLeft(sizeof(R));
    }
    template <Raw R>
    requires traits::ConstImplicationWith<T, R>
    [[nodiscard]] bool CastAndRemoveLeftCheck(Ptr<R>& ptr)
    {
        if (!CastLeftCheck(ptr)) {
            return false;
        }
        RemoveLeft(sizeof(R));
        return true;
    }

    template <Raw R>
    requires traits::ConstImplicationWith<T, R>
    void CastAndRemoveRight(Ptr<R>& ptr)
    {
        CastRight(ptr);
        RemoveRight(sizeof(R));
    }
    template <Raw R>
    requires traits::ConstImplicationWith<T, R>
    [[nodiscard]] bool CastAndRemoveRightCheck(Ptr<R>& ptr)
    {
        if (!CastRightCheck(ptr)) {
            return false;
        }
        RemoveRight(sizeof(R));
        return true;
    }

    template <RawByte U = T>
    requires traits::ConstImplicationWith<T, U>
    U* GetData() const
    {
        return ptr_.template Get<U>();
    }

    traits::CopyConstType<T, Byte>* GetByteData() const
    {
        return ptr_.GetByte();
    }

    traits::CopyConstType<T, Char>* GetCharData() const
    {
        return ptr_.GetChar();
    }

    template <Raw R = T>
    requires traits::ConstImplicationWith<T, R>
    Ptr<R> GetPointer() const
    {
        return Ptr<R>{ ptr_ };
    }

    Size GetSize() const
    {
        return size_;
    }

    template <RawByte U = T>
    U* Begin() const
    {
        return ptr_.template Get<U>();
    }

    template <RawByte U = T>
    U* End() const
    {
        Ptr<T> end = ptr_ + size_;
        return end.template Get<U>();
    }

#ifdef RAWBYTES_ENABLE_STD_SPAN
    template <RawByte U = T>
    requires traits::ConstImplicationWith<T, U>
    std::span<U> ToSpan() const
    {
        return std::span<U>{ ptr_.template Get<U>(), static_cast<typename std::span<U>::size_type>(size_) };
    }
#endif

#ifdef RAWBYTES_ENABLE_STD_STRING_VIEW
    template <RawByte U = T>
    requires std::is_trivially_default_constructible_v<U>
    std::basic_string_view<U> ToView() const
    {
        return std::basic_string_view<U>{ ptr_.template Get<traits::CopyConstType<T, U>>(),
                                          static_cast<typename std::basic_string_view<U>::size_type>(size_) };
    }
    std::string_view ToStringView() const
    {
        return std::string_view{ ptr_.GetChar(), static_cast<std::string_view::size_type>(size_) };
    }
#endif
private:
    Ptr<T> ptr_;
    Size size_{ 0 };
};

RAWBYTES_NAMESPACE_END
