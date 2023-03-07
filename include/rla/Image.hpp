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

#include <rla/Bitmap.hpp>
#include <cstddef>
#include <optional>
#include <string>
#include <vector>
#include <rlm/color/concepts.hpp>

namespace rl
{
    template<rl::color_channel C>
    struct color_g;
    template<rl::color_channel C>
    struct color_ga;
    template<rl::color_channel C>
    struct color_rgb;
    template<rl::color_channel C>
    struct color_rgba;

    class Image : public rl::Bitmap
    {
        protected:
            void shrink_data();
            void reserve_data(std::size_t capacity);
            void free_data() noexcept;

        protected:
            std::size_t capacity = 0;

        public:
            class Row : public rl::Bitmap::Row
            {
                protected:
                    void shrink_data();
                    void reserve_data(std::size_t capacity);
                    void free_data() noexcept;

                protected:
                    std::size_t capacity = 0;

                public:
                    using rl::Bitmap::Row::Row;

                    constexpr Row() noexcept = default;
                    Row(std::size_t capacity);
                    Row(std::size_t width, rl::Bitmap::Depth depth, rl::Bitmap::Color color);
                    template<rl::color_channel C>
                    Row(const rl::color_g<C>& color, std::size_t width);
                    template<rl::color_channel C>
                    Row(const rl::color_ga<C>& color, std::size_t width);
                    template<rl::color_channel C>
                    Row(const rl::color_rgb<C>& color, std::size_t width);
                    template<rl::color_channel C>
                    Row(const rl::color_rgba<C>& color, std::size_t width);
                    ~Row() noexcept override;

                    void Clear() noexcept;
                    void ShrinkToFit();
                    void Reserve(std::size_t capacity);
                    std::size_t GetCapacity() const noexcept;
                    void Create(std::size_t width, rl::Bitmap::Depth depth, rl::Bitmap::Color color);
                    template<rl::color_channel C>
                    void Create(const rl::color_g<C>& color, std::size_t width);
                    template<rl::color_channel C>
                    void Create(const rl::color_ga<C>& color, std::size_t width);
                    template<rl::color_channel C>
                    void Create(const rl::color_rgb<C>& color, std::size_t width);
                    template<rl::color_channel C>
                    void Create(const rl::color_rgba<C>& color, std::size_t width);
            };

        public:
            using rl::Bitmap::Bitmap;

            constexpr Image() noexcept = default;
            Image(std::size_t capacity);
            Image(std::size_t width, std::size_t height, std::size_t page_count, rl::Bitmap::Depth depth, rl::Bitmap::Color color);
            template<rl::color_channel C>
            Image(const rl::color_g<C>& color, std::size_t width, std::size_t height, std::size_t page_count);
            template<rl::color_channel C>
            Image(const rl::color_ga<C>& color, std::size_t width, std::size_t height, std::size_t page_count);
            template<rl::color_channel C>
            Image(const rl::color_rgb<C>& color, std::size_t width, std::size_t height, std::size_t page_count);
            template<rl::color_channel C>
            Image(const rl::color_rgba<C>& color, std::size_t width, std::size_t height, std::size_t page_count);
            ~Image() noexcept override;

            void Clear() noexcept;
            void ShrinkToFit();
            void Reserve(std::size_t capacity);
            std::size_t GetCapacity() const noexcept;
            void Create(std::size_t width, std::size_t height, std::size_t page_count, rl::Bitmap::Depth depth, rl::Bitmap::Color color);
            template<rl::color_channel C>
            void Create(const rl::color_g<C>& color, std::size_t width, std::size_t height, std::size_t page_count);
            template<rl::color_channel C>
            void Create(const rl::color_ga<C>& color, std::size_t width, std::size_t height, std::size_t page_count);
            template<rl::color_channel C>
            void Create(const rl::color_rgb<C>& color, std::size_t width, std::size_t height, std::size_t page_count);
            template<rl::color_channel C>
            void Create(const rl::color_rgba<C>& color, std::size_t width, std::size_t height, std::size_t page_count);
            void Load(const rl::Png& png, std::optional<rl::Bitmap::Depth> depth_o = std::nullopt, std::optional<rl::Bitmap::Color> color_o = std::nullopt);
            void Load(std::string_view path, std::optional<rl::Bitmap::Depth> depth_o = std::nullopt, std::optional<rl::Bitmap::Color> color_o = std::nullopt);
    };
}