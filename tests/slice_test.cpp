#include <gtest/gtest.h>

#define RAWBYTES_ENABLE_STD_SPAN
#define RAWBYTES_ENABLE_STD_STRING_VIEW
#include <rawbytes/slice.hpp>

namespace {

#pragma pack(push, 1)

struct Data
{
    int a{ 0 };
    int b{ 0 };
    int c{ 0 };
};

#pragma pack(pop)

static std::string_view kStr = "small str";

TEST(Slice, Null)
{
    rawbytes::Slice<rawbytes::Byte> null_byte_slice1;
    EXPECT_TRUE(null_byte_slice1.IsEmpty());
    EXPECT_EQ(null_byte_slice1.GetData(), nullptr);
    EXPECT_EQ(null_byte_slice1.GetData(), null_byte_slice1.GetByteData());
    EXPECT_EQ(null_byte_slice1.GetSize(), 0);

    rawbytes::Slice<rawbytes::Byte> null_byte_slice2(nullptr, 4);
    EXPECT_TRUE(null_byte_slice2.IsEmpty());
    EXPECT_EQ(null_byte_slice2.GetData(), nullptr);
    EXPECT_EQ(null_byte_slice2.GetData(), null_byte_slice2.GetByteData());
    EXPECT_EQ(null_byte_slice2.GetSize(), 0);
}

TEST(Slice, ViewConstruct)
{
    rawbytes::Slice<const rawbytes::Char> empty_slice(std::string_view{});
    EXPECT_TRUE(empty_slice.IsEmpty());
    EXPECT_EQ(empty_slice.GetData(), nullptr);
    EXPECT_EQ(empty_slice.GetSize(), 0);
    EXPECT_EQ(empty_slice.ToStringView(), std::string_view{});

    rawbytes::Slice<const rawbytes::Char> char_slice(kStr);
    EXPECT_FALSE(char_slice.IsEmpty());
    EXPECT_EQ(char_slice.GetData(), kStr.data());
    EXPECT_EQ(char_slice.GetSize(), kStr.size());
    EXPECT_EQ(char_slice.ToStringView(), kStr);

    rawbytes::Slice<const rawbytes::Byte> byte_slice(kStr);
    EXPECT_FALSE(byte_slice.IsEmpty());
    EXPECT_EQ(byte_slice.GetCharData(), kStr.data());
    EXPECT_EQ(byte_slice.GetSize(), kStr.size());
    EXPECT_EQ(byte_slice.ToStringView(), kStr);
}

TEST(Slice, Remove)
{
    rawbytes::Slice<const rawbytes::Char> empty_slice;
    EXPECT_TRUE(empty_slice.RemoveLeftCheck(0));
    EXPECT_TRUE(empty_slice.RemoveRightCheck(0));
    EXPECT_FALSE(empty_slice.RemoveLeftCheck(1));
    EXPECT_FALSE(empty_slice.RemoveRightCheck(1));
}

TEST(Slice, CopyTo)
{
    rawbytes::Slice<const rawbytes::Char> char_slice(kStr);

    rawbytes::Char c;
    // CopyToLeft
    EXPECT_TRUE(char_slice.CopyToLeftCheck(&c));
    EXPECT_EQ(c, kStr.front());
    // CopyToRight
    EXPECT_TRUE(char_slice.CopyToRightCheck(&c));
    EXPECT_EQ(c, kStr.back());
    // CopyToMid
    for (rawbytes::Pos idx = 0; idx < char_slice.GetSize(); idx += 1) {
        EXPECT_TRUE(char_slice.CopyToMidCheck(idx, &c));
        EXPECT_EQ(c, kStr[idx]);
    }

    EXPECT_FALSE(char_slice.CopyToMidCheck(char_slice.GetSize(), &c));
}

TEST(Slice, Peek)
{
    rawbytes::Slice<const rawbytes::Char> char_slice(kStr);
    
    rawbytes::Slice<const rawbytes::Char> sub_char_slice;
    // PeekLeft
    EXPECT_TRUE(char_slice.PeekLeftCheck(5, sub_char_slice));
    EXPECT_EQ(sub_char_slice.ToStringView(), kStr.substr(0, 5));
    // PeekMid
    EXPECT_TRUE(char_slice.PeekMidCheck(5, 1, sub_char_slice));
    EXPECT_EQ(sub_char_slice.ToStringView(), kStr.substr(5, 1));
    // PeekRight
    EXPECT_TRUE(char_slice.PeekRightCheck(3, sub_char_slice));
    EXPECT_EQ(sub_char_slice.ToStringView(), kStr.substr(6));
}

TEST(Slice, TakeLeft)
{
    rawbytes::Slice<const rawbytes::Char> slice(kStr);
    
    rawbytes::Slice<const rawbytes::Char> sub_slice;
    EXPECT_TRUE(slice.TakeLeftCheck(5, sub_slice));
    // slice - " str", sub - "small"
    EXPECT_EQ(sub_slice.ToStringView(), kStr.substr(0, 5));
    EXPECT_EQ(slice.ToStringView(), kStr.substr(5));

    EXPECT_TRUE(slice.TakeLeftCheck(1, sub_slice));
    // slice - "str", sub - " "
    EXPECT_EQ(sub_slice.ToStringView(), kStr.substr(5, 1));
    EXPECT_EQ(slice.ToStringView(), kStr.substr(6));

    EXPECT_TRUE(slice.TakeLeftCheck(3, sub_slice));
    // slice - "", sub - "str"
    EXPECT_EQ(sub_slice.ToStringView(), kStr.substr(6));
    EXPECT_TRUE(slice.IsEmpty());

    EXPECT_FALSE(slice.TakeLeftCheck(6, sub_slice));
}

TEST(Slice, TakeRight)
{
    rawbytes::Slice<const rawbytes::Char> slice(kStr);
    
    rawbytes::Slice<const rawbytes::Char> sub_slice;
    EXPECT_TRUE(slice.TakeRightCheck(3, sub_slice));
    // slice - "small ", sub - "str"
    EXPECT_EQ(sub_slice.ToStringView(), kStr.substr(6));
    EXPECT_EQ(slice.ToStringView(), kStr.substr(0, 6));

    EXPECT_TRUE(slice.TakeRightCheck(1, sub_slice));
    // slice - "small", sub - " "
    EXPECT_EQ(sub_slice.ToStringView(), kStr.substr(5, 1));
    EXPECT_EQ(slice.ToStringView(), kStr.substr(0, 5));

    EXPECT_TRUE(slice.TakeRightCheck(5, sub_slice));
    // slice - "", sub - "small"
    EXPECT_EQ(sub_slice.ToStringView(), kStr.substr(0, 5));
    EXPECT_TRUE(slice.IsEmpty());

    EXPECT_FALSE(slice.TakeRightCheck(6, sub_slice));
}

}
