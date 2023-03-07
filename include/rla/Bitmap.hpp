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

#pragma once

#include <rlm/cellular/cell_box2.hpp>
#include <cstddef>
#include <string>
#include <optional>

namespace rl
{
    class Image;
    class Png;

    class Bitmap
    {
        public:
            enum class Depth
            {
                Octuple = 0,
                Sexdecuple = 1,
                Normalized = 2,
                Default = Octuple
            };

            enum class Color
            {
                G = 0,
                Ga = 1,
                Rgb = 2,
                Rgba = 3,
                Default = Rgb
            };

            using byte_t = std::byte;

            using octuple_t = std::uint8_t;
            using sexdecuple_t = std::uint16_t;
            using normalized_t = float;

            class Row;
            class View;

            class Row
            {
            protected:
                rl::Bitmap::byte_t* data;
                std::size_t width = 0;
                rl::Bitmap::Depth depth = rl::Bitmap::Depth::Default;
                rl::Bitmap::Color color = rl::Bitmap::Color::Default;

            public:
                class View
                {
                    protected:
                        const rl::Bitmap::byte_t* data;
                        std::size_t width = 0;
                        rl::Bitmap::Depth depth = rl::Bitmap::Depth::Default;
                        rl::Bitmap::Color color = rl::Bitmap::Color::Default; 

                    public:
                        constexpr View() noexcept = default;
                        constexpr View(
                            const rl::Bitmap::byte_t* data,
                            std::size_t width,
                            rl::Bitmap::Depth depth,
                            rl::Bitmap::Color color
                        ) noexcept;
                        constexpr View(const rl::Bitmap::Row& row) noexcept;
                        constexpr rl::Bitmap::Row::View& operator=(const rl::Bitmap::Row& row) noexcept;

                        constexpr std::size_t GetWidth() const noexcept;
                        constexpr rl::Bitmap::Depth GetDepth() const noexcept;
                        constexpr rl::Bitmap::Color GetColor() const noexcept;
                        constexpr const rl::Bitmap::byte_t* GetData() const noexcept;
                        constexpr const rl::Bitmap::byte_t* GetData(std::size_t x, std::size_t channel = 0) const noexcept;
                        constexpr std::size_t GetChannelSize() const noexcept;
                        constexpr std::size_t GetBitDepth() const noexcept;
                        constexpr std::size_t GetSize() const noexcept;
                        constexpr std::optional<std::size_t> GetByteIndex(std::size_t x, std::size_t channel = 0) const noexcept;
                };

            public:
                constexpr Row() noexcept = default;
                constexpr Row(
                    rl::Bitmap::byte_t* data,
                    std::size_t width,
                    rl::Bitmap::Depth depth,
                    rl::Bitmap::Color color
                ) noexcept;
                virtual ~Row() noexcept = default;
                
                constexpr std::size_t GetWidth() const noexcept;
                constexpr rl::Bitmap::Depth GetDepth() const noexcept;
                constexpr rl::Bitmap::Color GetColor() const noexcept;
                constexpr rl::Bitmap::byte_t* GetData() const noexcept;
                constexpr rl::Bitmap::byte_t* GetData(std::size_t x, std::size_t channel = 0) const noexcept;
                constexpr std::size_t GetChannelSize() const noexcept;
                constexpr std::size_t GetBitDepth() const noexcept;
                constexpr std::size_t GetSize() const noexcept;
                constexpr std::optional<std::size_t> GetByteIndex(std::size_t x, std::size_t channel = 0) const noexcept;
                constexpr void Blit(const rl::Bitmap::Row::View& row);
            };

            class View
            {
                protected:
                    const rl::Bitmap::byte_t* data = nullptr;
                    std::size_t width = 0;
                    std::size_t height = 0;
                    std::size_t page_count = 0;
                    rl::Bitmap::Depth depth = rl::Bitmap::Depth::Default;
                    rl::Bitmap::Color color = rl::Bitmap::Color::Default;
                    std::size_t row_offset = 0;
                    std::size_t page_offset = 0;

                public:
                    constexpr View() noexcept = default;
                    constexpr View(
                        const rl::Bitmap::byte_t* data,
                        std::size_t width,
                        std::size_t height,
                        std::size_t page_count,
                        rl::Bitmap::Depth depth,
                        rl::Bitmap::Color color,
                        std::optional<std::size_t> row_offset_o = std::nullopt,
                        std::optional<std::size_t> page_offset_o = std::nullopt
                    ) noexcept;
                    constexpr View(const rl::Bitmap& bitmap) noexcept;
                    constexpr rl::Bitmap::View& operator=(const rl::Bitmap& bitmap) noexcept;

                    constexpr std::size_t GetWidth() const noexcept;
                    constexpr std::size_t GetHeight() const noexcept;
                    constexpr std::size_t GetPageCount() const noexcept;
                    constexpr std::size_t GetChannelSize() const noexcept;
                    constexpr rl::Bitmap::Depth GetDepth() const noexcept;
                    constexpr rl::Bitmap::Color GetColor() const noexcept;
                    constexpr std::size_t GetBitDepth() const noexcept;
                    constexpr std::size_t GetRowOffset() const noexcept;
                    constexpr std::size_t GetPageOffset() const noexcept;
                    constexpr const rl::Bitmap::byte_t* GetData() const noexcept;
                    constexpr const rl::Bitmap::byte_t* GetData(std::size_t x, std::size_t y = 0, std::size_t page = 0, std::size_t channel = 0) const noexcept;
                    constexpr std::size_t GetChannelCount() const noexcept;
                    constexpr std::size_t GetRowSize() const noexcept;
                    constexpr std::size_t GetPixelSize() const noexcept;
                    constexpr std::size_t GetPageSize() const noexcept;
                    constexpr std::size_t GetSize() const noexcept;
                    constexpr std::optional<std::size_t> GetByteIndex(std::size_t x, std::size_t y = 0, std::size_t page = 0, std::size_t channel = 0) const noexcept;
                    constexpr bool GetIsEmpty() const noexcept;
                    constexpr const rl::Bitmap::View GetBitmapView(std::optional<rl::Bitmap::Depth> fake_depth_o = std::nullopt, std::optional<rl::Bitmap::Color> fake_color_o = std::nullopt) const;
                    constexpr const rl::Bitmap::View GetBitmapView(std::size_t x, std::size_t y, std::size_t page, std::size_t width, std::size_t height, std::size_t page_count, std::optional<rl::Bitmap::Depth> fake_depth_o = std::nullopt, std::optional<rl::Bitmap::Color> fake_color_o = std::nullopt) const;
                    constexpr const rl::Bitmap::Row::View GetRowView(std::size_t y, std::size_t page, std::optional<rl::Bitmap::Depth> fake_depth_o = std::nullopt, std::optional<rl::Bitmap::Color> fake_color_o = std::nullopt) const;
                    void Save(std::string_view path, std::size_t page = 0);
            };

        public:
            static constexpr std::size_t GetChannelCount(rl::Bitmap::Color color) noexcept;
            static constexpr std::size_t GetChannelSize(rl::Bitmap::Depth depth) noexcept;
            static constexpr std::size_t GetBitDepth(rl::Bitmap::Depth depth) noexcept;
            static constexpr rl::Bitmap::Depth GetDepth(std::size_t bit_depth) noexcept;
            static constexpr std::size_t GetPixelSize(rl::Bitmap::Depth depth, rl::Bitmap::Color color) noexcept;
            static constexpr std::size_t GetRowSize(std::size_t width, rl::Bitmap::Depth depth, rl::Bitmap::Color color) noexcept;
            static constexpr std::size_t GetPageSize(std::size_t width, std::size_t height, rl::Bitmap::Depth depth, rl::Bitmap::Color color) noexcept;
            static constexpr std::size_t GetSize(std::size_t width, std::size_t height, std::size_t pages, rl::Bitmap::Depth depth, rl::Bitmap::Color color) noexcept;
            static constexpr std::optional<std::size_t> GetByteIndex(std::size_t width, std::size_t height, std::size_t pages, rl::Bitmap::Depth depth, rl::Bitmap::Color color, std::size_t row_offset, std::size_t page_offset, std::size_t x, std::size_t y, std::size_t page, std::size_t channel) noexcept;
    
        protected:
            rl::Bitmap::byte_t* data = nullptr;
            std::size_t width = 0;
            std::size_t height = 0;
            std::size_t page_count = 0;
            rl::Bitmap::Depth depth = rl::Bitmap::Depth::Default;
            rl::Bitmap::Color color = rl::Bitmap::Color::Default;
            std::size_t row_offset = 0;
            std::size_t page_offset = 0;

            constexpr bool blit_fits(const rl::cell_box2<int>& blit_box, std::size_t page) const noexcept;
        public:
            constexpr Bitmap() noexcept = default;
            constexpr Bitmap(
                rl::Bitmap::byte_t* data,
                std::size_t width,
                std::size_t height,
                std::size_t page_count,
                rl::Bitmap::Depth depth,
                rl::Bitmap::Color color,
                std::optional<std::size_t> row_offset_o = std::nullopt,
                std::optional<std::size_t> page_offset_o = std::nullopt
            ) noexcept;
            virtual ~Bitmap() noexcept = default;

            constexpr std::size_t GetWidth() const noexcept;
            constexpr std::size_t GetHeight() const noexcept;
            constexpr std::size_t GetPageCount() const noexcept;
            constexpr std::size_t GetChannelSize() const noexcept;
            constexpr rl::Bitmap::Depth GetDepth() const noexcept;
            constexpr rl::Bitmap::Color GetColor() const noexcept;
            constexpr std::size_t GetBitDepth() const noexcept;
            constexpr std::size_t GetRowOffset() const noexcept;
            constexpr std::size_t GetPageOffset() const noexcept;
            constexpr rl::Bitmap::byte_t* GetData() const noexcept;
            constexpr rl::Bitmap::byte_t* GetData(std::size_t x, std::size_t y = 0, std::size_t page = 0, std::size_t channel = 0) const noexcept;            
            constexpr std::size_t GetChannelCount() const noexcept;
            constexpr std::size_t GetRowSize() const noexcept;
            constexpr std::size_t GetPixelSize() const noexcept;
            constexpr std::size_t GetPageSize() const noexcept;
            constexpr std::size_t GetSize() const noexcept;
            constexpr std::optional<std::size_t> GetByteIndex(std::size_t x, std::size_t y = 0, std::size_t page = 0, std::size_t channel = 0) const noexcept;
            constexpr bool GetIsEmpty() const noexcept;
            constexpr rl::Bitmap GetBitmap(std::optional<rl::Bitmap::Depth> fake_depth_o = std::nullopt, std::optional<rl::Bitmap::Color> fake_color_o = std::nullopt);
            constexpr rl::Bitmap GetBitmap(std::size_t x, std::size_t y, std::size_t page, std::size_t width, std::size_t height, std::size_t page_count, std::optional<rl::Bitmap::Depth> fake_depth_o = std::nullopt, std::optional<rl::Bitmap::Color> fake_color_o = std::nullopt);
            constexpr rl::Bitmap::View GetBitmapView(std::optional<rl::Bitmap::Depth> fake_depth_o = std::nullopt, std::optional<rl::Bitmap::Color> fake_color_o = std::nullopt) const;
            constexpr rl::Bitmap::View GetBitmapView(std::size_t x, std::size_t y, std::size_t page, std::size_t width, std::size_t height, std::size_t page_count, std::optional<rl::Bitmap::Depth> fake_depth_o = std::nullopt, std::optional<rl::Bitmap::Color> fake_color_o = std::nullopt) const;
            constexpr rl::Bitmap::Row GetRow(std::size_t y, std::size_t page, std::optional<rl::Bitmap::Depth> fake_depth_o = std::nullopt, std::optional<rl::Bitmap::Color> fake_color_o = std::nullopt) const;
            constexpr const rl::Bitmap::Row::View GetRowView(std::size_t y, std::size_t page, std::optional<rl::Bitmap::Depth> fake_depth_o = std::nullopt, std::optional<rl::Bitmap::Color> fake_color_o = std::nullopt) const;
            void Save(std::string_view path, std::size_t page = 0);
            constexpr void Blit(const rl::Bitmap::View& bitmap, std::size_t x, std::size_t y, std::size_t page);
            void Blit(const rl::Png& png, std::size_t x, std::size_t y, std::size_t page);
            void Blit(std::string_view path, std::size_t x, std::size_t y, std::size_t page);
    };
}

#include <rla/detail/Bitmap.inl>
#include <rla/detail/Bitmap_View.inl>
#include <rla/detail/Bitmap_Row.inl>
#include <rla/detail/Bitmap_Row_View.inl>