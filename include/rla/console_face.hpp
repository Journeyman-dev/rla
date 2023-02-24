#pragma once

#include <rla/types.hpp>
#include <vector>
#include <map>

namespace rl
{
    struct console_face
    {
        bool letterboxed_wide = false;
        bool letterboxed_tall = false;
        std::vector<float> texture_coordinates = std::vector<float>();
        std::map<rl::codepoint_t, rl::glyph_t> codepoint_map;
    };
}