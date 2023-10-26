#pragma once

#include <cinttypes>
#include <string>

#include "Vec.h"
#include "Color.h"
#include "Util.h"

constexpr size_t CHANNELS = 3;
constexpr bool FLIP_IMAGE = true;

class Image
{
private:
    uint8_t* m_Buffer;
    size_t m_Width;
    size_t m_Height;

public:
    Image(size_t width, size_t height);
    Image(size_t width, size_t height, const Color &bgColor);
    Image(const Image &other); // Copy CTOR
    Image& operator=(const Image& other); // Copy Assignment
    Image(Image &&other) noexcept; // Move CTOR
    Image& operator=(Image&& other) noexcept; // Move Assignment
    ~Image();

    inline size_t GetWidth() const { return m_Width; }
    inline size_t GetHeight() const { return m_Height; }

    Color& Pixel(size_t row, size_t col) const;
    Color& Pixel(size_t row, size_t col);
    inline Color& operator()(size_t x, size_t y) const { return Pixel(y, x); }
    inline Color& operator()(size_t x, size_t y) { return Pixel(y, x); }

    inline void Write(const std::string& path) { Write(path.c_str()); }
    void Write(const char *path);
    inline void Clear() { std::memset(m_Buffer, 0, m_Width * m_Height * CHANNELS); }
};