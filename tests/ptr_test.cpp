#include <gtest/gtest.h>

#include <rawbytes/ptr.hpp>

namespace {

#pragma pack(push, 1)

struct Data
{
    int a{ 0 };
    int b{ 0 };
    int c{ 0 };
};

#pragma pack(pop)

static rawbytes::Char kCharData[] = "small str";

TEST(Ptr, NullConstruct)
{
    rawbytes::Ptr<rawbytes::Byte> raw_null_byte_ptr;
    EXPECT_TRUE(raw_null_byte_ptr == nullptr);
    EXPECT_FALSE(raw_null_byte_ptr != nullptr);
    EXPECT_EQ(raw_null_byte_ptr.Get(), nullptr);
    EXPECT_EQ(raw_null_byte_ptr.Get(), raw_null_byte_ptr.GetByte());
    EXPECT_EQ(raw_null_byte_ptr.GetSizeBytes(), 0);
}

TEST(Ptr, ImplicitConstruct)
{
    rawbytes::Char* char_data_ptr = kCharData;
    rawbytes::Ptr<rawbytes::Char> raw_char_ptr(char_data_ptr);

    EXPECT_TRUE(raw_char_ptr != nullptr);
    EXPECT_EQ(raw_char_ptr.Get(), kCharData);
    EXPECT_EQ(raw_char_ptr.GetSizeBytes(), 1);
    rawbytes::Ptr<const rawbytes::Char> raw_const_char_ptr(char_data_ptr);
    EXPECT_TRUE(raw_const_char_ptr != nullptr);
    EXPECT_EQ(raw_const_char_ptr.Get(), kCharData);
    EXPECT_EQ(raw_const_char_ptr.GetSizeBytes(), 1);

    rawbytes::Ptr<const rawbytes::Char> raw_const_char_ptr2(raw_char_ptr);
    EXPECT_TRUE(raw_const_char_ptr2 != nullptr);
    EXPECT_EQ(raw_const_char_ptr2.Get(), kCharData);
    EXPECT_EQ(raw_const_char_ptr2.GetSizeBytes(), 1);   
}

TEST(Ptr, CastConstruct)
{
    rawbytes::Char* char_data_ptr = kCharData;
    const rawbytes::Char* const_char_data_ptr = char_data_ptr;
    
    rawbytes::Ptr<rawbytes::Byte> raw_byte_ptr(char_data_ptr);
    EXPECT_EQ(raw_byte_ptr.Get<rawbytes::Char>(), char_data_ptr);
    EXPECT_EQ(raw_byte_ptr.GetChar(), char_data_ptr);

    rawbytes::Ptr<const rawbytes::Byte> raw_const_byte_ptr(const_char_data_ptr);
    EXPECT_EQ(raw_const_byte_ptr.Get<const rawbytes::Char>(), const_char_data_ptr);
    EXPECT_EQ(raw_const_byte_ptr.GetChar(), const_char_data_ptr);

    rawbytes::Ptr<rawbytes::Char> raw_char_ptr(raw_byte_ptr);
    EXPECT_EQ(raw_char_ptr.Get(), char_data_ptr);
    EXPECT_EQ(raw_char_ptr.GetChar(), char_data_ptr);

    rawbytes::Ptr<const rawbytes::Char> raw_const_char_ptr(raw_const_byte_ptr);
    EXPECT_EQ(raw_const_char_ptr.Get(), const_char_data_ptr);
    EXPECT_EQ(raw_const_char_ptr.GetChar(), const_char_data_ptr);

    EXPECT_EQ(raw_byte_ptr, raw_const_char_ptr);
    EXPECT_EQ(raw_const_char_ptr, raw_byte_ptr);
}

TEST(Ptr, RawConstruct)
{
    Data data;
    data.a = 1;
    data.b = 2;
    data.c = 3;

    rawbytes::Ptr<const Data> const_data_ptr(&data);
    EXPECT_EQ(const_data_ptr.Get(), &data);
    EXPECT_EQ(const_data_ptr.GetSizeBytes(), sizeof(Data));
    EXPECT_EQ(const_data_ptr->a, 1);
    EXPECT_EQ(const_data_ptr->b, 2);
    EXPECT_EQ(const_data_ptr->c, 3);

    rawbytes::Ptr<Data> data_ptr(&data);
    EXPECT_EQ(data_ptr.Get(), &data);
    EXPECT_EQ(data_ptr.GetSizeBytes(), sizeof(Data));

    data_ptr->a = 4;
    data_ptr->b = 5;
    data_ptr->c = 6;
    EXPECT_EQ(data_ptr->a, 4);
    EXPECT_EQ(data_ptr->b, 5);
    EXPECT_EQ(data_ptr->c, 6);
    EXPECT_EQ(const_data_ptr->a, 4);
    EXPECT_EQ(const_data_ptr->b, 5);
    EXPECT_EQ(const_data_ptr->c, 6);
    EXPECT_EQ(data.a, 4);
    EXPECT_EQ(data.b, 5);
    EXPECT_EQ(data.c, 6);
}

}
