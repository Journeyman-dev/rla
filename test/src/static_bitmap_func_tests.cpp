// SPDX-FileCopyrightText: 2023 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

/*
    Copyright (c) 2023 Daniel Aimé Valcour
    Permission is hereby granted, free of charge, to any person obtaining a copy of
    this software and associated documentation files (the "Software"), to deal in
    the Software without restriction, including without limitation the rights to
    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
    the Software, and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <catch2/catch_all.hpp>
#include <rla/Bitmap.hpp>

// clang-format off

TEST_CASE("Bitmap channel count is determined given a rl::Bitmap::Color")
{
    CHECK(rl::Bitmap::GetChannelCount(rl::Bitmap::Color::G)    == 1);
    CHECK(rl::Bitmap::GetChannelCount(rl::Bitmap::Color::Ga)   == 2);
    CHECK(rl::Bitmap::GetChannelCount(rl::Bitmap::Color::Rgb)  == 3);
    CHECK(rl::Bitmap::GetChannelCount(rl::Bitmap::Color::Rgba) == 4);
}

TEST_CASE("Bitmap channel size is determined given a rl::Bitmap::Depth")
{
    CHECK(rl::Bitmap::GetChannelSize(rl::Bitmap::Depth::Octuple)    == 1);
    CHECK(rl::Bitmap::GetChannelSize(rl::Bitmap::Depth::Sexdecuple) == 2);
    CHECK(rl::Bitmap::GetChannelSize(rl::Bitmap::Depth::Normalized) == 4);
}

TEST_CASE("Bitmap bit depth is determined given a rl::Bitmap::Depth")
{
    CHECK(rl::Bitmap::GetBitDepth(rl::Bitmap::Depth::Octuple)    == 8);
    CHECK(rl::Bitmap::GetBitDepth(rl::Bitmap::Depth::Sexdecuple) == 16);
    CHECK(rl::Bitmap::GetBitDepth(rl::Bitmap::Depth::Normalized) == 32);
}

TEST_CASE("Bitmap pixel size is determined given a rl::Bitmap::Depth and a rl::Bitmap::Color")
{
    CHECK(rl::Bitmap::GetPixelSize(rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G)    == 1);
    CHECK(rl::Bitmap::GetPixelSize(rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Ga)   == 2);
    CHECK(rl::Bitmap::GetPixelSize(rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgb)  == 3);
    CHECK(rl::Bitmap::GetPixelSize(rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgba) == 4);
    CHECK(rl::Bitmap::GetPixelSize(rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::G)    == 2);
    CHECK(rl::Bitmap::GetPixelSize(rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Ga)   == 4);
    CHECK(rl::Bitmap::GetPixelSize(rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgb)  == 6);
    CHECK(rl::Bitmap::GetPixelSize(rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgba) == 8);
    CHECK(rl::Bitmap::GetPixelSize(rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::G)    == 4);
    CHECK(rl::Bitmap::GetPixelSize(rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Ga)   == 8);
    CHECK(rl::Bitmap::GetPixelSize(rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgb)  == 12);
    CHECK(rl::Bitmap::GetPixelSize(rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgba) == 16);
}

TEST_CASE("Bitmap row size is determined given a width, a rl::BitmapDepth, and a rl::Bitmap::Color")
{
    CHECK(rl::Bitmap::GetRowSize(1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G)    == 1);
    CHECK(rl::Bitmap::GetRowSize(1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Ga)   == 2);
    CHECK(rl::Bitmap::GetRowSize(1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgb)  == 3);
    CHECK(rl::Bitmap::GetRowSize(1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgba) == 4);
    CHECK(rl::Bitmap::GetRowSize(1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::G)    == 2);
    CHECK(rl::Bitmap::GetRowSize(1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Ga)   == 4);
    CHECK(rl::Bitmap::GetRowSize(1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgb)  == 6);
    CHECK(rl::Bitmap::GetRowSize(1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgba) == 8);
    CHECK(rl::Bitmap::GetRowSize(1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::G)    == 4);
    CHECK(rl::Bitmap::GetRowSize(1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Ga)   == 8);
    CHECK(rl::Bitmap::GetRowSize(1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgb)  == 12);
    CHECK(rl::Bitmap::GetRowSize(1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgba) == 16);
    CHECK(rl::Bitmap::GetRowSize(512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G)    == 512);
    CHECK(rl::Bitmap::GetRowSize(512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Ga)   == 1024);
    CHECK(rl::Bitmap::GetRowSize(512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgb)  == 1536);
    CHECK(rl::Bitmap::GetRowSize(512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgba) == 2048);
    CHECK(rl::Bitmap::GetRowSize(512, rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::G)    == 1024);
    CHECK(rl::Bitmap::GetRowSize(512, rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Ga)   == 2048);
    CHECK(rl::Bitmap::GetRowSize(512, rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgb)  == 3072);
    CHECK(rl::Bitmap::GetRowSize(512, rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgba) == 4096);
    CHECK(rl::Bitmap::GetRowSize(512, rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::G)    == 2048);
    CHECK(rl::Bitmap::GetRowSize(512, rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Ga)   == 4096);
    CHECK(rl::Bitmap::GetRowSize(512, rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgb)  == 6144);
    CHECK(rl::Bitmap::GetRowSize(512, rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgba) == 8192);
}

TEST_CASE("Bitmap page size is determined given a width, a height, a rl::Bitmap::Depth, and a rl::Bitmap::Color")
{
    CHECK(rl::Bitmap::GetPageSize(1,   1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G)    == 1);
    CHECK(rl::Bitmap::GetPageSize(1,   1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Ga)   == 2);
    CHECK(rl::Bitmap::GetPageSize(1,   1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgb)  == 3);
    CHECK(rl::Bitmap::GetPageSize(1,   1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgba) == 4);
    CHECK(rl::Bitmap::GetPageSize(1,   1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::G)    == 2);
    CHECK(rl::Bitmap::GetPageSize(1,   1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Ga)   == 4);
    CHECK(rl::Bitmap::GetPageSize(1,   1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgb)  == 6);
    CHECK(rl::Bitmap::GetPageSize(1,   1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgba) == 8);
    CHECK(rl::Bitmap::GetPageSize(1,   1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::G)    == 4);
    CHECK(rl::Bitmap::GetPageSize(1,   1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Ga)   == 8);
    CHECK(rl::Bitmap::GetPageSize(1,   1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgb)  == 12);
    CHECK(rl::Bitmap::GetPageSize(1,   1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgba) == 16);
    CHECK(rl::Bitmap::GetPageSize(512, 1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G)    == 512);
    CHECK(rl::Bitmap::GetPageSize(512, 1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Ga)   == 1024);
    CHECK(rl::Bitmap::GetPageSize(512, 1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgb)  == 1536);
    CHECK(rl::Bitmap::GetPageSize(512, 1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgba) == 2048);
    CHECK(rl::Bitmap::GetPageSize(512, 1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::G)    == 1024);
    CHECK(rl::Bitmap::GetPageSize(512, 1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Ga)   == 2048);
    CHECK(rl::Bitmap::GetPageSize(512, 1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgb)  == 3072);
    CHECK(rl::Bitmap::GetPageSize(512, 1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgba) == 4096);
    CHECK(rl::Bitmap::GetPageSize(512, 1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::G)    == 2048);
    CHECK(rl::Bitmap::GetPageSize(512, 1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Ga)   == 4096);
    CHECK(rl::Bitmap::GetPageSize(512, 1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgb)  == 6144);
    CHECK(rl::Bitmap::GetPageSize(512, 1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgba) == 8192);
    CHECK(rl::Bitmap::GetPageSize(512, 512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G)    == 262144);
    CHECK(rl::Bitmap::GetPageSize(512, 512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Ga)   == 524288);
    CHECK(rl::Bitmap::GetPageSize(512, 512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgb)  == 786432);
    CHECK(rl::Bitmap::GetPageSize(512, 512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgba) == 1048576);
    CHECK(rl::Bitmap::GetPageSize(512, 512, rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::G)    == 524288);
    CHECK(rl::Bitmap::GetPageSize(512, 512, rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Ga)   == 1048576);
    CHECK(rl::Bitmap::GetPageSize(512, 512, rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgb)  == 1572864);
    CHECK(rl::Bitmap::GetPageSize(512, 512, rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgba) == 2097152);
    CHECK(rl::Bitmap::GetPageSize(512, 512, rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::G)    == 1048576);
    CHECK(rl::Bitmap::GetPageSize(512, 512, rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Ga)   == 2097152);
    CHECK(rl::Bitmap::GetPageSize(512, 512, rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgb)  == 3145728);
    CHECK(rl::Bitmap::GetPageSize(512, 512, rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgba) == 4194304);
}

TEST_CASE("Bitmap size is determined given a width, a height, a page count, a rl::Bitmap::Depth, and a rl::Bitmap::Color")
{
    CHECK(rl::Bitmap::GetSize(1,   1,   1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G)    == 1);
    CHECK(rl::Bitmap::GetSize(1,   1,   1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Ga)   == 2);
    CHECK(rl::Bitmap::GetSize(1,   1,   1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgb)  == 3);
    CHECK(rl::Bitmap::GetSize(1,   1,   1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgba) == 4);
    CHECK(rl::Bitmap::GetSize(1,   1,   1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::G)    == 2);
    CHECK(rl::Bitmap::GetSize(1,   1,   1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Ga)   == 4);
    CHECK(rl::Bitmap::GetSize(1,   1,   1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgb)  == 6);
    CHECK(rl::Bitmap::GetSize(1,   1,   1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgba) == 8);
    CHECK(rl::Bitmap::GetSize(1,   1,   1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::G)    == 4);
    CHECK(rl::Bitmap::GetSize(1,   1,   1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Ga)   == 8);
    CHECK(rl::Bitmap::GetSize(1,   1,   1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgb)  == 12);
    CHECK(rl::Bitmap::GetSize(1,   1,   1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgba) == 16);
    CHECK(rl::Bitmap::GetSize(512, 1,   1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G)    == 512);
    CHECK(rl::Bitmap::GetSize(512, 1,   1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Ga)   == 1024);
    CHECK(rl::Bitmap::GetSize(512, 1,   1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgb)  == 1536);
    CHECK(rl::Bitmap::GetSize(512, 1,   1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgba) == 2048);
    CHECK(rl::Bitmap::GetSize(512, 1,   1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::G)    == 1024);
    CHECK(rl::Bitmap::GetSize(512, 1,   1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Ga)   == 2048);
    CHECK(rl::Bitmap::GetSize(512, 1,   1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgb)  == 3072);
    CHECK(rl::Bitmap::GetSize(512, 1,   1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgba) == 4096);
    CHECK(rl::Bitmap::GetSize(512, 1,   1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::G)    == 2048);
    CHECK(rl::Bitmap::GetSize(512, 1,   1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Ga)   == 4096);
    CHECK(rl::Bitmap::GetSize(512, 1,   1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgb)  == 6144);
    CHECK(rl::Bitmap::GetSize(512, 1,   1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgba) == 8192);
    CHECK(rl::Bitmap::GetSize(512, 512, 1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G)    == 262144);
    CHECK(rl::Bitmap::GetSize(512, 512, 1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Ga)   == 524288);
    CHECK(rl::Bitmap::GetSize(512, 512, 1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgb)  == 786432);
    CHECK(rl::Bitmap::GetSize(512, 512, 1,   rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgba) == 1048576);
    CHECK(rl::Bitmap::GetSize(512, 512, 1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::G)    == 524288);
    CHECK(rl::Bitmap::GetSize(512, 512, 1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Ga)   == 1048576);
    CHECK(rl::Bitmap::GetSize(512, 512, 1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgb)  == 1572864);
    CHECK(rl::Bitmap::GetSize(512, 512, 1,   rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgba) == 2097152);
    CHECK(rl::Bitmap::GetSize(512, 512, 1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::G)    == 1048576);
    CHECK(rl::Bitmap::GetSize(512, 512, 1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Ga)   == 2097152);
    CHECK(rl::Bitmap::GetSize(512, 512, 1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgb)  == 3145728);
    CHECK(rl::Bitmap::GetSize(512, 512, 1,   rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgba) == 4194304);
    CHECK(rl::Bitmap::GetSize(512, 512, 255, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G)    == 66846720);
    CHECK(rl::Bitmap::GetSize(512, 512, 255, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Ga)   == 133693440);
    CHECK(rl::Bitmap::GetSize(512, 512, 255, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgb)  == 200540160);
    CHECK(rl::Bitmap::GetSize(512, 512, 255, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Rgba) == 267386880);
    CHECK(rl::Bitmap::GetSize(512, 512, 255, rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::G)    == 133693440);
    CHECK(rl::Bitmap::GetSize(512, 512, 255, rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Ga)   == 267386880);
    CHECK(rl::Bitmap::GetSize(512, 512, 255, rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgb)  == 401080320);
    CHECK(rl::Bitmap::GetSize(512, 512, 255, rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgba) == 534773760);
    CHECK(rl::Bitmap::GetSize(512, 512, 255, rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::G)    == 267386880);
    CHECK(rl::Bitmap::GetSize(512, 512, 255, rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Ga)   == 534773760);
    CHECK(rl::Bitmap::GetSize(512, 512, 255, rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgb)  == 802160640);
    CHECK(rl::Bitmap::GetSize(512, 512, 255, rl::Bitmap::Depth::Normalized, rl::Bitmap::Color::Rgba) == 1069547520);
}

TEST_CASE("A Bitmap byte index is calculated given a width, a height, a page count, a rl::Bitmap::Depth, a rl::Bitmap::Color, a row offset, a page offset, an x, a y, a page, and a channel")
{
    CHECK(rl::Bitmap::GetByteIndex(512, 512, 512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G,    512, 262144, 512,   0,   0, 0) == std::nullopt);
    CHECK(rl::Bitmap::GetByteIndex(512, 512, 512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G,    512, 262144,   0, 512,   0, 0) == std::nullopt);
    CHECK(rl::Bitmap::GetByteIndex(512, 512, 512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G,    512, 262144,   0,   0, 512, 0) == std::nullopt);
    CHECK(rl::Bitmap::GetByteIndex(512, 512, 512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G,    512, 262144,   0,   0,   0, 1) == std::nullopt);
    CHECK(rl::Bitmap::GetByteIndex(512, 512, 512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G,    512, 262144,   0,   0,   0, 0) == 0);
    CHECK(rl::Bitmap::GetByteIndex(512, 512, 512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G,    512, 262144, 101,   0,   0, 0) == 101);
    CHECK(rl::Bitmap::GetByteIndex(512, 512, 512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G,    512, 262144,   0, 101,   0, 0) == 51712);
    CHECK(rl::Bitmap::GetByteIndex(512, 512, 512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::G,    512, 262144,   0,   0, 101, 0) == 26476544);
    CHECK(rl::Bitmap::GetByteIndex(512, 512, 512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Ga,  1024, 524288,   0,   0,   0, 0) == 0);
    CHECK(rl::Bitmap::GetByteIndex(512, 512, 512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Ga,  1024, 524288, 101,   0,   0, 0) == 202);
    CHECK(rl::Bitmap::GetByteIndex(512, 512, 512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Ga,  1024, 524288,   0, 101,   0, 0) == 103424);
    CHECK(rl::Bitmap::GetByteIndex(512, 512, 512, rl::Bitmap::Depth::Octuple,    rl::Bitmap::Color::Ga,  1024, 524288,   0,   0, 101, 0) == 52953088);
    CHECK(rl::Bitmap::GetByteIndex(512, 512, 512, rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgb, 2000, 80000,    0,   0,   0, 0) == 0);
    CHECK(rl::Bitmap::GetByteIndex(512, 512, 512, rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgb, 2000, 80000,  101,   0,   0, 1) == 608);
    CHECK(rl::Bitmap::GetByteIndex(512, 512, 512, rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgb, 2000, 80000,    0, 101,   0, 2) == 202004);
    CHECK(rl::Bitmap::GetByteIndex(512, 512, 512, rl::Bitmap::Depth::Sexdecuple, rl::Bitmap::Color::Rgb, 2000, 80000,    0,   0, 101, 0) == 8080000);
}