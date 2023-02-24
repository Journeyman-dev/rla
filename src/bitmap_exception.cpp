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

#include <rla/bitmap_exception.hpp>
#include "png_wrapper.h"
#include <ostream>
#include <sstream>

rl::bitmap_exception::bitmap_exception(rl::bitmap_exception::Error error) noexcept
    : error(error)
{}

const char* rl::bitmap_exception::what() const noexcept
{
    std::stringstream ss("");
    ss << "png load or save error: \"" << this->error << "\"";
    return ss.str().data();
}

rl::bitmap_exception::Error rl::bitmap_exception::get_error() const noexcept
{
    return this->error;
}

std::ostream& rl::operator<<(std::ostream& os, rl::bitmap_exception::Error error)
{
    if (error == rl::bitmap_exception::Error::BlitOutOfBitmap)
    {
        // This error is not from png_wrapper.h
        os << "blit out of bitmap";
    }
    else if (
        static_cast<pngwresult_t>(error) >= PNGW_RESULT_OK &&
        static_cast<pngwresult_t>(error) < PNGW_RESULT_COUNT)
    {
        os << PNGW_RESULT_DESCRIPTIONS[static_cast<pngwresult_t>(error)];
    }
    else
    {
        os << "unkown error";
    }
    return os;
}
