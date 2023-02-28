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
#include <rla/bitmap_types.hpp>
#include <rlm/cellular/cell_box2.hpp>
#include <cstddef>


namespace rl
{
    class MutableBitmapView;
    class MutableBitmapRowView;

    class Png;
    class MutableBitmap : public rl::Bitmap
    {
        protected:
            bool blit_fits(const rl::cell_box2<int>& blit_box, std::size_t page) const noexcept;

        public:
            using rl::Bitmap::Bitmap;

            virtual unsigned char* GetMutableData() noexcept = 0;
            unsigned char* GetMutableData(std::size_t x, std::size_t y = 0, std::size_t page = 0, std::size_t channel = 0) noexcept;
            rl::MutableBitmapView GetMutableView() noexcept;
            rl::MutableBitmapView GetMutableView(std::size_t x, std::size_t y, std::size_t page, std::size_t width, std::size_t height, std::size_t page_count) noexcept;
            rl::MutableBitmapRowView GetMutableRowView(std::size_t y, std::size_t page) noexcept;
            void BlitBitmap(const rl::Bitmap& bitmap, std::size_t x, std::size_t y, std::size_t page);
            void BlitPng(const rl::Png& png, std::size_t x, std::size_t y, std::size_t page);
            void BlitPng(std::string_view path, std::size_t x, std::size_t y, std::size_t page);
    };
}