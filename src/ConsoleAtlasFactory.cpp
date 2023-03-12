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

#include <rla/ConsoleAtlasFactory.hpp>
#include "console_atlas_source_key.hpp"
#include <rld/except.hpp>
#include <rlm/cellular/shape_edges.hpp>
#include <numeric>
#include <unordered_map>
#include <set>

rl::console_atlas rl::ConsoleAtlasFactory::Create(const rl::console_atlas::layout& layout)
{
    this->png_images.clear();
    this->font_sources.clear();
    this->pack_boxes.clear();
    this->glyph_identifiers.clear();
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
        if (face_layout.letterboxed)
        {
            atlas.has_letterboxing = true;
        }
    }
    if (atlas.has_letterboxing && atlas.tile_width % 2 != 0)
    {
        throw rl::runtime_error("console atlas can not be letterboxed when width is not divisible by 2");
    }
    this->png_images.reserve(layout.png_sources.size());
    for (const auto& png_source : layout.png_sources)
    {
        this->png_images.emplace_back(png_source);
    }
    this->font_sources.reserve(layout.font_sources.size());
    for (const auto& font_source : layout.font_sources)
    {
        this->font_sources.emplace_back(font_source);
    }
    atlas.faces.reserve(layout.faces.size());
    const auto max_pack_box_count =
        std::accumulate(
            layout.faces.begin(),
            layout.faces.end(),
            0,
            [](int count, rl::console_atlas::layout::face face)
            {
                return face.glyphs.size() + count;
            }
        );
    this->pack_boxes.reserve(
        max_pack_box_count
    );
    this->glyph_identifiers.reserve(
        max_pack_box_count
    );
    std::unordered_map<rl::console_atlas_source_key, std::size_t> source_map;
    std::vector<rl::console_atlas_source_key> source_vector;
    for (const auto& face_layout : layout.faces)
    {
        auto& face = atlas.faces.emplace_back();
        face.letterboxed = face_layout.letterboxed;
        const auto tile_width = 
            (face.letterboxed) ?
            atlas.tile_width / 2 :
            atlas.tile_width;
        std::set<wchar_t> found_codepoints;
        for (const auto& glyph_layout : face_layout.glyphs)
        {
            rl::console_atlas_source_key source_key;
            source_key.letterboxed = face.letterboxed;
            source_key.top_left = glyph_layout.top_left;
            source_key.source_i = glyph_layout.source_i;
            source_key.source = glyph_layout.source;
            if (glyph_layout.source == rl::console_atlas::layout::Source::Font)
            {
                // we only care about the codepoint for the hash gen if this is a for a font glyph
                source_key.codepoint = glyph_layout.codepoint;
            }
            if (source_map.contains(source_key))
            {
                this->glyph_identifiers.push_back(source_map.at(source_key));
            }
            else
            {
                const auto actual_source_i = this->pack_boxes.size();
                source_map[source_key] = actual_source_i;
                this->glyph_identifiers.push_back(actual_source_i);
                this->pack_boxes.emplace_back(this->pack_boxes.size(), tile_width, atlas.tile_height);
                source_vector.push_back(source_key);
            }
            if (found_codepoints.contains(glyph_layout.codepoint))
            {
                throw rl::runtime_error("duplicate of same codepoint found in console atlas face");
            }
            found_codepoints.insert(glyph_layout.codepoint);
        }
        // after the glyphs are done, add the remaining font glyphs with codepoints not already claimed
        std::wstring font_codepoints = this->font_sources[face_layout.font].GetAllCodepoints();
        for (const auto codepoint : font_codepoints)
        {
            if (!found_codepoints.contains(codepoint))
            {
                rl::console_atlas_source_key source_key;
                source_key.letterboxed = face.letterboxed;;
                source_key.source_i = face_layout.font;
                source_key.source = rl::console_atlas::layout::Source::Font;
                source_key.codepoint = codepoint;
                if (source_map.contains(source_key))
                {
                    this->glyph_identifiers.push_back(source_map.at(source_key));
                }
                else
                {
                    const auto actual_source_i = this->pack_boxes.size();
                    source_map[source_key] = actual_source_i;
                    this->glyph_identifiers.push_back(actual_source_i);
                    this->pack_boxes.emplace_back(this->pack_boxes.size(), tile_width, atlas.tile_height);
                    source_vector.push_back(source_key);
                }
            }
        }
    }
    this->packer.Pack(this->pack_boxes);
    atlas.image.Create(this->packer.GetWidth(), this->packer.GetHeight(), this->packer.GetPageCount(), rl::Bitmap::Depth::Octuple, rl::Bitmap::Color::G);
    std::sort(
        this->pack_boxes.begin(),
        this->pack_boxes.end(),
        [](rl::pack_box<int> a, rl::pack_box<int> b)
        {
            return a.identifier < b.identifier;
        }
    );
    for (std::size_t box_i = 0; box_i < this->pack_boxes.size(); box_i++)
    {
        const auto& pack_box = this->pack_boxes[box_i];
        const auto& source = source_vector[box_i];
        rl::Bitmap::View view;
        if (source.source == rl::console_atlas::layout::Source::Bitmap)
        {
            view = layout.bitmap_sources[source.source_i].GetBitmapView(source.top_left.x, source.top_left.y, 0, pack_box.box.width, pack_box.box.height, 1);
        }
        else if (source.source == rl::console_atlas::layout::Source::Png)
        {
            view = png_images[source.source_i].GetBitmapView(source.top_left.x, source.top_left.y, 0, pack_box.box.width, pack_box.box.height, 1);
        }
        else if (source.source == rl::console_atlas::layout::Source::Font)
        {
            auto& font = this->font_sources[source.source_i];
            font.SetPixelSizes(pack_box.box.width, pack_box.box.height);
            font.LoadChar(source.codepoint);
            view = font.GetCharBitmap();
        }
        atlas.image.Blit(view, pack_box.box.x, pack_box.box.y, pack_box.page);
    }
    std::size_t glyph_identifier_i = 0;
    for (std::size_t face_i = 0; face_i < layout.faces.size(); face_i++)
    {
        const auto& face_layout = layout.faces[face_i];
        auto& face = atlas.faces[face_i];
        const std::size_t coordinates_per_stpqp = 5;
        face.texture_coordinates.reserve(face_layout.glyphs.size() * coordinates_per_stpqp);
        for (const auto& glyph_layout : face_layout.glyphs)
        {
            const auto identifer = this->glyph_identifiers[glyph_identifier_i];
            const auto& pack_box = this->pack_boxes[glyph_identifier_i];
            face.texture_coordinates.push_back( // s
                static_cast<float>(atlas.image.GetWidth()) /
                static_cast<float>(rl::left_x(pack_box.box))
            );
            face.texture_coordinates.push_back( // t
                static_cast<float>(atlas.image.GetWidth()) /
                static_cast<float>(rl::right_x(pack_box.box))
            );
            face.texture_coordinates.push_back( // p
                static_cast<float>(atlas.image.GetHeight()) /
                static_cast<float>(rl::top_y(pack_box.box))
            );
            face.texture_coordinates.push_back( // q
                static_cast<float>(atlas.image.GetHeight()) /
                static_cast<float>(rl::bottom_y(pack_box.box))
            );
            face.texture_coordinates.push_back(static_cast<float>(pack_box.page));
        }
    }
    return atlas;
}