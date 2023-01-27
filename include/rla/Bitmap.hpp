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

#ifndef RLA_BITMAP_HPP
#define RLA_BITMAP_HPP

#include <cstddef>
#include <optional>
#include <string>

namespace rl
{
    class Bitmap
    {
        public:
            enum class Color
            {
                G,
                Ga,
                Rgb,
                Rgba
            };
            static constexpr std::size_t GetChannelCount(rl::Bitmap::Color color) noexcept;
            static constexpr std::size_t GetChannelSize(std::size_t bit_depth) noexcept;
            static constexpr std::size_t GetBitDepth(std::size_t channel_size) noexcept;
            static constexpr std::size_t GetPixelSize(rl::Bitmap::Color color, std::size_t byte_depth) noexcept;
            static constexpr std::size_t GetRowSize(rl::Bitmap::Color color, std::size_t byte_depth, std::size_t width) noexcept;
            static constexpr std::size_t GetPageSize(rl::Bitmap::Color color, std::size_t byte_depth, std::size_t width, std::size_t height) noexcept;
            static constexpr std::size_t GetSize(rl::Bitmap::Color color, std::size_t byte_depth, std::size_t width, std::size_t height, std::size_t pages = 1) noexcept;
            static constexpr std::optional<std::size_t> GetByteIndex(rl::Bitmap::Color color, std::size_t byte_depth, std::size_t width, std::size_t height, std::size_t pages, std::size_t x, std::size_t y, std::size_t page, std::size_t channel) noexcept;
        public:
            virtual rl::Bitmap::Color GetColor() const noexcept = 0;
            virtual std::size_t GetWidth() const noexcept = 0;
            virtual std::size_t GetHeight() const noexcept = 0;
            virtual std::size_t GetPages() const noexcept = 0;
            virtual std::size_t GetChannelSize() const noexcept = 0;
            std::size_t GetBitDepth() const noexcept;
            virtual const unsigned char* GetData() const noexcept = 0;
            const unsigned char* GetData(std::size_t x, std::size_t y = 0, std::size_t page = 0, std::size_t channel = 0) const noexcept;
            std::size_t GetChannelCount() const noexcept;
            std::size_t GetRowSize() const noexcept;
            std::size_t GetPageSize() const noexcept;
            std::size_t GetSize() const noexcept;
            std::optional<std::size_t> GetByteIndex(std::size_t x, std::size_t y = 0, std::size_t page = 0, std::size_t channel = 0) const noexcept;
            void SavePng(std::string_view path, std::size_t page = 0);
    };
}

#include <rla/detail/Bitmap.inl>

#endif
