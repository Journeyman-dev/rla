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

#include <rla/BitmapRow.hpp>
#include <rla/Bitmap.hpp>
#include <rla/bitmap_types.hpp>
#include <cstddef>
#include <optional>

const rl::bitmap_byte_t* rl::BitmapRow::GetData(std::size_t x, std::size_t channel) const noexcept
{
    const auto byte_index_o = this->GetByteIndex(x, channel);
    if (!byte_index_o.has_value())
    {
        return nullptr;
    }
    return
        this->GetData() +
        byte_index_o.value();
}

std::size_t rl::BitmapRow::GetChannelSize() const noexcept
{
    return
        rl::Bitmap::GetChannelSize(
            this->GetDepth()
        );
}

std::size_t rl::BitmapRow::GetBitDepth() const noexcept
{
    return
        rl::Bitmap::GetBitDepth(
            this->GetDepth()
        );
}

std::size_t rl::BitmapRow::GetSize() const noexcept
{
    return
        rl::Bitmap::GetRowSize(
            this->GetWidth(),
            this->GetDepth(),
            this->GetColor()
        );
}

std::optional<std::size_t> rl::BitmapRow::GetByteIndex(std::size_t x, std::size_t channel) const noexcept
{
    return
        rl::Bitmap::GetByteIndex(
            this->GetWidth(),
            0,
            0,
            this->GetDepth(),
            this->GetColor(),
            0,
            0,
            x,
            0,
            0,
            channel
        );
}