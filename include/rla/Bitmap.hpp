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

#pragma once

#include <rla/BitmapColor.hpp>
#include <cstddef>
#include <optional>
#include <string>

namespace rl
{
    class BitmapView;

    class Bitmap
    {
        public:
            static constexpr std::size_t GetChannelCount(rl::BitmapColor color) noexcept;
            static constexpr std::size_t GetChannelSize(std::size_t bit_depth) noexcept;
            static constexpr std::size_t GetBitDepth(std::size_t channel_size) noexcept;
            static constexpr std::size_t GetPixelSize(std::size_t channel_size, rl::BitmapColor color) noexcept;
            static constexpr std::size_t GetRowSize(std::size_t width, std::size_t channel_size, rl::BitmapColor color) noexcept;
            static constexpr std::size_t GetPageSize(std::size_t width, std::size_t height, std::size_t channel_size, rl::BitmapColor color) noexcept;
            static constexpr std::size_t GetSize(std::size_t width, std::size_t height, std::size_t pages, std::size_t channel_size, rl::BitmapColor color) noexcept;
            static constexpr std::optional<std::size_t> GetByteIndex(std::size_t width, std::size_t height, std::size_t pages, std::size_t channel_size, rl::BitmapColor color, std::size_t row_offset, std::size_t page_offset, std::size_t x, std::size_t y, std::size_t page, std::size_t channel) noexcept;
            static constexpr void ConvertRow(
                unsigned char* source,
                unsigned char* destination,
                std::size_t width,
                std::size_t source_channel_size,
                rl::BitmapColor source_color,
                std::size_t destination_channel_size,
                rl::BitmapColor destination_color
            ) noexcept;

        public:
            virtual std::size_t GetWidth() const noexcept = 0;
            virtual std::size_t GetHeight() const noexcept = 0;
            virtual std::size_t GetPages() const noexcept = 0;
            virtual std::size_t GetChannelSize() const noexcept = 0;
            virtual rl::BitmapColor GetColor() const noexcept = 0;
            std::size_t GetBitDepth() const noexcept;
            virtual std::size_t GetRowOffset() const noexcept;
            virtual std::size_t GetPageOffset() const noexcept;
            virtual const unsigned char* GetData() const noexcept = 0;
            const unsigned char* GetData(std::size_t x, std::size_t y = 0, std::size_t page = 0, std::size_t channel = 0) const noexcept;
            std::size_t GetChannelCount() const noexcept;
            std::size_t GetRowSize() const noexcept;
            std::size_t GetPageSize() const noexcept;
            std::size_t GetSize() const noexcept;
            std::optional<std::size_t> GetByteIndex(std::size_t x, std::size_t y = 0, std::size_t page = 0, std::size_t channel = 0) const noexcept;
            bool GetIsEmpty() const noexcept;
            rl::BitmapView GetView() const noexcept;
            rl::BitmapView GetView(std::size_t x, std::size_t y, std::size_t page, std::size_t width, std::size_t height, std::size_t pages) const noexcept;
            void SavePng(std::string_view path, std::size_t page = 0);
    };
}

#include <rla/detail/Bitmap.inl>