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

#include <rla/PngException.hpp>
#include "png_wrapper.h"
#include <ostream>
#include <sstream>

rl::PngException::PngException(rl::PngException::Error error) noexcept
    : error(error)
{}

const char* rl::PngException::what() const noexcept
{
    std::stringstream ss("");
    ss << "png load or save error: \"" << this->error << "\"";
    return ss.str().data();
}

rl::PngException::Error rl::PngException::get_error() const noexcept
{
    return this->error;
}

std::ostream& rl::operator<<(std::ostream& os, rl::PngException::Error error)
{
    if (error == rl::PngException::Error::BlitOutOfImage)
    {
        // This error is not from png_wrapper.h
        os << "blit out of bitmap";
    }
    else
    {
        os << PNGW_RESULT_DESCRIPTIONS[static_cast<pngwresult_t>(error)];
    }
    return os;
}
