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

#include <rla/console_atlas.hpp>
#include <rlm/cellular/bin_packing.hpp>
#include <rla/Font.hpp>
#include <rla/Png.hpp>
#include <rld/except.hpp>
#include <vector>
#include <span>
#include <algorithm>

rl::console_atlas rl::console_atlas::Create(const rl::console_atlas::layout& layout)
{
    rl::console_atlas atlas;
    if (layout.tile_width <= 0 || layout.tile_height <= 0)
    {
        throw rl::runtime_error("invalid console atlas tile dimensions");
    }
    atlas.tile_width = layout.tile_width;
    atlas.tile_height = layout.tile_height;
    if (layout.faces.empty())
    {
        return atlas;
    }
    for (const auto& face_layout : layout.faces)
    {
        if (face_layout.letterboxed_wide)
        {
            atlas.has_letterboxing_wide = true;
        }
        if (face_layout.letterboxed_tall)
        {
            atlas.has_letterboxing_tall = true;
        }
    }
    if (
        (atlas.has_letterboxing_wide && atlas.tile_width % 2 != 0) ||
        (atlas.has_letterboxing_tall && atlas.tile_height % 2 != 0)
    )
    {
        throw rl::runtime_error("console atlas tile dimension can not be letterboxed wide when not divisible by 2");
    }
    std::vector<rl::Png> png_sources;
    png_sources.reserve(layout.png_sources.size());
    std::vector<rl::Image> png_images;
    png_images.reserve(png_sources.size());
    for (const auto& png_source : layout.png_sources)
    {
        auto& png = png_sources.emplace_back(png_source);
        auto& image = png_images.emplace_back();
        image.Load(png);
    }
    std::vector<rl::Font> font_sources;
    font_sources.reserve(layout.font_sources.size());
    for (const auto& font_source : layout.font_sources)
    {
        font_sources.emplace_back(font_source);
    }
    atlas.faces.reserve(layout.faces.size());
    std::vector<rl::pack_box<int>> pack_boxes;
    {
        std::size_t pack_boxes_capacity = 0;
        for (const auto& face_layout : layout.faces)
        {
            pack_boxes_capacity += face_layout.glyphs.size();
        }
        pack_boxes.reserve(pack_boxes_capacity);
    }
    for (const auto& face_layout : layout.faces)
    {
        auto& face = atlas.faces.emplace_back();
        face.letterboxed_wide = face_layout.letterboxed_wide;
        face.letterboxed_tall = face_layout.letterboxed_tall;
        const auto tile_width = 
            (face.letterboxed_wide) ? 
            atlas.tile_width / 2 :
            atlas.tile_width;
        const auto tile_height =
            (face.letterboxed_tall) ?
            atlas.tile_height / 2 :
            atlas.tile_height;
        for (const auto& glyph_layout : face_layout.glyphs)
        {
            pack_boxes.emplace_back(pack_boxes.size(), tile_width, tile_height);
        }
    }
    rl::Packer<int> packer;
    packer.Pack(pack_boxes);
    atlas.image.Create(
        packer.GetWidth(),
        packer.GetHeight(),
        packer.GetPageCount(),
        rl::Bitmap::Depth::Octuple,
        rl::Bitmap::Color::G
    );
    std::sort(
        pack_boxes.begin(),
        pack_boxes.end(),
        [](rl::pack_box<int> a, rl::pack_box<int> b)
        {
            return a.identifier > b.identifier;
        }
    );
    {
        std::size_t pack_box_i = 0;
        for (const auto& face_layout : layout.faces)
        {
            auto& face = atlas.faces.emplace_back();
            for (const auto& glyph_layout : face_layout.glyphs)
            {
                const auto& pack_box = pack_boxes[pack_box_i++];
                if (glyph_layout.source == rl::console_atlas::layout::Source::Bitmap)
                {
                    auto bitmap = layout.bitmap_sources[glyph_layout.source_i];
                    if (glyph_layout.top_left_o.has_value())
                    {
                        const auto& top_left = glyph_layout.top_left_o.value();
                        bitmap = bitmap.GetBitmapView(
                            static_cast<std::size_t>(top_left.x),
                            static_cast<std::size_t>(top_left.y),
                            0,
                            static_cast<std::size_t>(pack_box.box.width),
                            static_cast<std::size_t>(pack_box.box.height),
                            1
                        );
                    }
                    atlas.image.Blit(
                        bitmap,
                        static_cast<std::size_t>(pack_box.box.x),
                        static_cast<std::size_t>(pack_box.box.y),
                        static_cast<std::size_t>(pack_box.page)
                    );
                }
                else if (glyph_layout.source == rl::console_atlas::layout::Source::Png)
                {
                    rl::Bitmap::View bitmap = png_images[glyph_layout.source_i];
                    if (glyph_layout.top_left_o.has_value())
                    {
                        const auto& top_left = glyph_layout.top_left_o.value();
                        bitmap = bitmap.GetBitmapView(
                            static_cast<std::size_t>(top_left.x),
                            static_cast<std::size_t>(top_left.y),
                            0,
                            static_cast<std::size_t>(pack_box.box.width),
                            static_cast<std::size_t>(pack_box.box.height),
                            1
                        );
                    }
                    atlas.image.Blit(
                        bitmap,
                        static_cast<std::size_t>(pack_box.box.x),
                        static_cast<std::size_t>(pack_box.box.y),
                        static_cast<std::size_t>(pack_box.page)
                    );
                }
                else if (glyph_layout.source == rl::console_atlas::layout::Source::Font)
                {
                    auto& font = font_sources[glyph_layout.source_i];
                    font.SetPixelSizes(pack_box.box.width, pack_box.box.height);
                    font.LoadChar(glyph_layout.codepoint_o.value());
                    auto bitmap = font.GetCharBitmap();
                    atlas.image.Blit(
                        bitmap,
                        static_cast<std::size_t>(pack_box.box.x),
                        static_cast<std::size_t>(pack_box.box.y),
                        static_cast<std::size_t>(pack_box.page)
                    );
                }
                // TODO assign texture coordinates
            }
        }
    }
   
    return atlas;
}