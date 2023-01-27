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

#ifndef RLA_IMAGE_HPP
#define RLA_IMAGE_HPP

#include <rla/MutableBitmap.hpp>
#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace rl
{
    class Image : public rl::MutableBitmap
    {
        private:
            rl::Bitmap::Color color;
            std::size_t channel_size;
            std::size_t width;
            std::size_t height;
            std::size_t pages;
            std::vector<unsigned char> data;

        public:
            using rl::MutableBitmap::MutableBitmap;

            rl::Bitmap::Color GetColor() const noexcept override;
            std::size_t GetWidth() const noexcept override;
            std::size_t GetHeight() const noexcept override;
            std::size_t GetPages() const noexcept override;
            std::size_t GetChannelSize() const noexcept override;
            unsigned char* GetMutableData() noexcept override;
            const unsigned char* GetData() const noexcept override;
            //void LoadPng(const rl::Png& png, std::optional<rl::Bitmap::Color> color = std::nullopt, std::optional<std::size_t> channel_count = std::nullopt);
            //void LoadPng(std::string_view path, std::optional<rl::Bitmap::Color> color, std::optional<std::size_t> channel_count);
    };
}

#endif
