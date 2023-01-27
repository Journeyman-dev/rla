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

#include <rla/Bitmap.hpp>
#include <rla/PngException.hpp>
#include <cstddef>
#include "png_wrapper.h"
#include "pngw_ext.hpp"

std::size_t rl::Bitmap::GetBitDepth() const noexcept
{
    return
        rl::Bitmap::GetBitDepth(
            this->GetChannelSize()
        );
}

const unsigned char* rl::Bitmap::GetData(std::size_t x, std::size_t y, std::size_t page, std::size_t channel) const noexcept
{
    const auto byte_index_o = this->GetByteIndex(x, y, page, channel);
    if (!byte_index_o.has_value())
    {
        return nullptr;
    }
    return
        this->GetData() +
        byte_index_o.value();
}

std::size_t rl::Bitmap::GetChannelCount() const noexcept
{
    return
        rl::Bitmap::GetChannelCount(
            this->GetColor()
        );
}

std::size_t rl::Bitmap::GetRowSize() const noexcept
{
    return
        rl::Bitmap::GetRowSize(
            this->GetColor(),
            this->GetChannelSize(),
            this->GetWidth()
        );
}

std::size_t rl::Bitmap::GetPageSize() const noexcept
{
    return
        rl::Bitmap::GetPageSize(
            this->GetColor(),
            this->GetChannelSize(),
            this->GetWidth(),
            this->GetHeight()
        );
}

std::size_t rl::Bitmap::GetSize() const noexcept
{
    return
        rl::Bitmap::GetSize(
            this->GetColor(),
            this->GetChannelSize(),
            this->GetWidth(),
            this->GetHeight(),
            this->GetPages()
        );
}

std::optional<std::size_t> rl::Bitmap::GetByteIndex(std::size_t x, std::size_t y, std::size_t page, std::size_t channel) const noexcept
{
    return
        rl::Bitmap::GetByteIndex(
            this->GetColor(),
            this->GetChannelSize(),
            this->GetWidth(),
            this->GetHeight(),
            this->GetPages(),
            x,
            y,
            page,
            channel
        );
}

void rl::Bitmap::SavePng(std::string_view path, std::size_t page)
{
    pngwresult_t result = pngwWriteFile(
        path.data(),
        this->GetData(0, 0, page, 0),
        PNGW_DEFAULT_ROW_OFFSET,
        this->GetWidth(),
        this->GetHeight(),
        this->GetBitDepth(),
        PNGW_COLOR_RGBA
    );
    if (result != PNGW_RESULT_OK)
    {
        throw rl::PngException(rl::pngw_result_to_png_exception_error(result));
    }
}
