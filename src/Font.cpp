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

#include <rla/Font.hpp>
#include <freetype/freetype.h>
#include <stdexcept>
#include <string>
#include <rla/Bitmap.hpp>
#include <rla/font_exception.hpp>

rl::Font::~Font() noexcept
{
    if (this->freetype_face != nullptr)
    {
        FT_Done_Face(this->freetype_face);
        this->freetype_face = nullptr;
    }
    if (this->freetype_library != nullptr)
    {
        FT_Done_FreeType(this->freetype_library);
        this->freetype_library = nullptr;
    }
}
            
rl::Font::Font(std::string_view path)
{
    this->Load(path);
}

void rl::Font::Load(std::string_view path)
{
    if (this->freetype_library == nullptr)
    {
        if (FT_Init_FreeType(&this->freetype_library))
        {
            throw rl::font_exception(rl::font_exception::Error::FreetypeInitializeFailure);
        }
    }
    if (this->freetype_face != nullptr)
    {
        FT_Done_Face(this->freetype_face);
        this->freetype_face = nullptr;
    }
    if (FT_New_Face(this->freetype_library, path.data(), 0, &this->freetype_face))
    {
        throw rl::font_exception(rl::font_exception::Error::FontLoadFailure);
    }
}

void rl::Font::Unload()
{
    if (this->freetype_face != nullptr)
    {
        FT_Done_Face(this->freetype_face);
        this->freetype_face = nullptr;
    }
}

bool rl::Font::IsLoaded() const noexcept
{
    return this->freetype_face != nullptr;
}

void rl::Font::SetPixelSizes(int width, int height)
{
    if (!this->IsLoaded())
    {
        throw rl::font_exception(rl::font_exception::Error::FontNotLoaded);
    }
    if (FT_Set_Pixel_Sizes(this->freetype_face, width, height))
    {
        throw std::runtime_error("failed to set pixel sizes");
    }
}

void rl::Font::LoadChar(char character)
{
    if (!this->IsLoaded())
    {
        throw rl::font_exception(rl::font_exception::Error::FontNotLoaded);
    }
    if (FT_Load_Char(this->freetype_face, character, FT_LOAD_RENDER))
    {
        throw std::runtime_error("failed to load character");
    }
}

rl::Bitmap::View rl::Font::GetCharBitmap() const
{
    if (!this->IsLoaded())
    {
        throw rl::font_exception(rl::font_exception::Error::GlyphLoadFailure);
    }
    return 
        rl::Bitmap::View(
            reinterpret_cast<const rl::Bitmap::byte_t*>(freetype_face->glyph->bitmap.buffer),
            this->freetype_face->glyph->bitmap.width,
            this->freetype_face->glyph->bitmap.rows,
            1,
            rl::Bitmap::Depth::Octuple,
            rl::Bitmap::Color::G,
            std::nullopt,
            std::nullopt
        );
}

std::wstring rl::Font::GetAllCodepoints() const
{
    std::wstring codepoints;
    codepoints.reserve(this->freetype_face->num_glyphs);
    FT_UInt glyph_index = 0;
    FT_ULong next_codepoint = FT_Get_First_Char(this->freetype_face, &glyph_index);
    while (glyph_index != 0)
    {
        codepoints.push_back(next_codepoint);
        next_codepoint = FT_Get_Next_Char(this->freetype_face, next_codepoint, &glyph_index);
    }
    return codepoints;
}