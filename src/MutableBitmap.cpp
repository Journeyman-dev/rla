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

#include <rla/MutableBitmap.hpp>
#include <rla/MutableBitmapView.hpp>
#include <rla/MutableBitmapRowView.hpp>
#include <rla/Png.hpp>
#include "png_wrapper.h"
#include "pngw_ext.hpp"
#include <rlm/cellular/cell_box2.hpp>
#include <rlm/cellular/does_contain.hpp>
#include <rla/bitmap_exception.hpp>
#include <rla/color_conversion.hpp>
#include <cstring>

bool rl::MutableBitmap::blit_fits(const rl::cell_box2<int>& blit_box, std::size_t page) const noexcept
{
    rl::cell_box2<int> this_box(
        0,
        0,
        static_cast<int>(this->GetWidth()),
        static_cast<int>(this->GetHeight())
    );
    return 
        page < this->GetPageCount() &&
        rl::does_contain(this_box, blit_box);
}

unsigned char* rl::MutableBitmap::GetMutableData(std::size_t x, std::size_t y, std::size_t page, std::size_t channel) noexcept
{
    const auto byte_index_o = this->GetByteIndex(x, y, page, channel);
    if (!byte_index_o.has_value())
    {
        return nullptr;
    }
    return
        this->GetMutableData() +
        byte_index_o.value();
}

rl::MutableBitmapView rl::MutableBitmap::GetMutableView() noexcept
{
    return
        this->GetMutableView(
            0,
            0,
            0,
            this->GetWidth(),
            this->GetHeight(),
            this->GetPageCount()
        );
}

rl::MutableBitmapView rl::MutableBitmap::GetMutableView(std::size_t x, std::size_t y, std::size_t page, std::size_t width, std::size_t height, std::size_t page_count) noexcept
{
    return
        rl::MutableBitmapView(
            this->GetMutableData(x, y, page, 0),
            width,
            height,
            page_count,
            this->GetDepth(),
            this->GetColor(),
            this->GetRowOffset(),
            this->GetPageOffset()
        );
}

rl::MutableBitmapRowView rl::MutableBitmap::GetMutableRowView(std::size_t y, std::size_t page) noexcept
{
    return
        rl::MutableBitmapRowView(
            this->GetMutableData(0, y, page, 0),
            this->GetWidth(),
            this->GetDepth(),
            this->GetColor()
        );
}

void rl::MutableBitmap::BlitBitmap(const rl::Bitmap& bitmap, std::size_t x, std::size_t y, std::size_t page)
{
    if (
        !this->blit_fits(
            rl::cell_box2<int>(
                x,
                y,
                bitmap.GetWidth(),
                bitmap.GetHeight()
            ),
            page
        )
    )
    {
        throw rl::bitmap_exception(rl::bitmap_exception::Error::BlitOutOfBitmap);
    }
    for (std::size_t blit_page = 0; blit_page < bitmap.GetPageCount(); blit_page++)
    {
        for (std::size_t blit_y = 0; blit_y < bitmap.GetHeight(); blit_y++)
        {
            for (std::size_t blit_x = 0; blit_x < bitmap.GetWidth(); blit_x++)
            {
                for (std::size_t blit_channel = 0; blit_channel < bitmap.GetChannelCount(); blit_channel++)
                {
                    std::memcpy(
                        this->GetMutableData(
                            blit_x + x,
                            blit_y + y,
                            blit_page + page,
                            blit_channel
                        ),
                        bitmap.GetData(
                            blit_x,
                            blit_y,
                            blit_page,
                            blit_channel
                        ),
                        bitmap.GetRowSize()
                    );
                }
            }
        }
    }
}

void rl::MutableBitmap::BlitPng(const rl::Png& png, std::size_t x, std::size_t y, std::size_t page)
{
    if (
        !this->blit_fits(
            rl::cell_box2<int>(
                x,
                y,
                png.GetWidth(),
                png.GetHeight()
            ),
            page
        )
    )
    {
        throw rl::bitmap_exception(rl::bitmap_exception::Error::BlitOutOfBitmap);
    }
    const auto png_color = rl::to_png_color(this->GetColor());
    pngwReadFile(
        png.GetPath().data(),
        this->GetMutableData(x, y, page, 0),
        this->GetRowOffset(),
        png.GetWidth(),
        png.GetHeight(),
        this->GetBitDepth(),
        rl::png_color_to_pngw_color(png_color)
    );
}

void rl::MutableBitmap::BlitPng(std::string_view path, std::size_t x, std::size_t y, std::size_t page)
{
    rl::Png png(path);
    this->BlitPng(png, x, y, page);
}
