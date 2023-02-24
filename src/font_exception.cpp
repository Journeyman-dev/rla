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

#include <rla/font_exception.hpp>
#include <stdexcept>
#include <ostream>
#include <sstream>

rl::font_exception::font_exception(rl::font_exception::Error error) noexcept
    : error(error)
{
}

const char* rl::font_exception::what() const noexcept
{
    std::stringstream ss("");
    ss << "font load or save error: \"" << this->error << "\"";
    return ss.str().data();
}

rl::font_exception::Error rl::font_exception::get_error() const noexcept
{
    return this->error;
}

std::ostream& rl::operator<<(std::ostream& os, rl::font_exception::Error error)
{
    switch (error)
    {
        case rl::font_exception::Error::FontNotLoaded:
            os << "font not loaded";
            break;
        case rl::font_exception::Error::FreetypeInitializeFailure:
            os << "error initializing freetype";
            break;
        case rl::font_exception::Error::FontLoadFailure:
            os << "failed to load font";
            break;
        case rl::font_exception::Error::GlyphLoadFailure:
            os << "failed to load glyph";
            break;
        default:
            os << "unkown error";
    }
    return os;
}