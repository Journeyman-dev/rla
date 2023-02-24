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

#include <rla/Image.hpp>
#include <rla/Png.hpp>
#include <rla/color_conversion.hpp>

unsigned char* rl::Image::GetMutableData() noexcept
{
    return this->data.data();
}

rl::BitmapColor rl::Image::GetColor() const noexcept
{
    return this->color;
}

std::size_t rl::Image::GetWidth() const noexcept
{
    return this->width;
}

std::size_t rl::Image::GetHeight() const noexcept
{
    return this->height;
}

std::size_t rl::Image::GetPages() const noexcept
{
    return this->pages;
}

std::size_t rl::Image::GetChannelSize() const noexcept
{
    return this->channel_size;
}

const unsigned char* rl::Image::GetData() const noexcept
{
    return this->data.data();
}

void rl::Image::Clear() noexcept
{
    this->data.clear();
    this->width = 0;
    this->height = 0;
    this->pages = 0;
    this->channel_size = 1;
    this->color = rl::BitmapColor::Rgba;
}

void rl::Image::ShrinkToFit()
{
    this->data.shrink_to_fit();
}

void rl::Image::Reserve(std::size_t bytes)
{
    this->data.reserve(bytes);
}

void rl::Image::Create(std::size_t width, std::size_t height, std::size_t pages, std::size_t channel_size, rl::BitmapColor color)
{
    this->Clear();
    const auto size = 
        rl::Bitmap::GetSize(
            width,
            height,
            pages,
            channel_size,
            color
        );
    this->data.resize(size, 0);
    this->width = width;
    this->height = height;
    this->pages = pages;
    this->color = color;
    this->channel_size = channel_size;
}

void rl::Image::LoadPng(const rl::Png& png, std::optional<std::size_t> channel_size_o, std::optional<rl::BitmapColor> color_o)
{
    const auto color = 
        color_o.value_or(
            rl::to_bitmap_color(png.GetColor())
        );
    const auto channel_size =
        channel_size_o.value_or(
            rl::Bitmap::GetChannelSize(png.GetBitDepth())
        );
    this->Create(png.GetWidth(), png.GetHeight(), 1, channel_size, color);
    this->BlitPng(png, 0, 0, 0);
}

void rl::Image::LoadPng(std::string_view path, std::optional<std::size_t> channel_size_o, std::optional<rl::BitmapColor> color_o)
{
    const auto png = rl::Png(path);
    this->LoadPng(png, channel_size_o, color_o);
}