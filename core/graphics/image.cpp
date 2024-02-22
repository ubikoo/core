//
// image.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

/// @brief stb image loader implementation
#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <cassert>
#include <cstring>
#include <array>
#include <string>
#include <sstream>
#include <fstream>

#include "core/graphics/graphics.h"
#include "image.h"

namespace Graphics {

/// -----------------------------------------------------------------------------
/// @brief Return a string with image properties.
///
std::string GetImagePropertiesString(
    const Image &image,
    const char *comment)
{
    std::ostringstream ss;
    if (comment != nullptr) {
        ss << comment << "\n";
    }
    ss << "width:    " << image.width       << "\n"
       << "height:   " << image.height      << "\n"
       << "bpp:      " << image.bpp         << "\n"
       << "channels: " << (image.bpp >> 3)  << "\n"
       << "pitch:    " << image.pitch       << "\n"
       << "size:     " << image.size        << "\n"
       << "bitmap:   " << static_cast<const void*>(&image.bitmap[0]) << "\n";
    return ss.str();
}

/// -----------------------------------------------------------------------------
/// @brief Create an image with specified size and pixel depth.
///
Image CreateImage(
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp)
{
    assert(width > 0);
    assert(height > 0);
    assert(bpp == 8 || bpp == 16 || bpp == 24 || bpp == 32);

    //
    // Set the image width and height in pixel units.
    // The pixel format is congrous with the bit depth (8 bits per channel).
    // Set the bitmap row stride in bytes, rounded to the next 32-bit boundary,
    // and with a buffer size in bytes.
    //
    Image image;
    image.width = width;
    image.height = height;
    image.bpp = bpp;
    image.format = (bpp ==  8 ? GL_RED  :
                    bpp == 16 ? GL_RG   :
                    bpp == 24 ? GL_RGB  :
                    bpp == 32 ? GL_RGBA : GL_NONE);
    image.pitch = 4 * (((width * bpp) + 31) / 32);
    image.size = height * image.pitch;
    image.bitmap.resize(image.size);
    return image;
}

/// -----------------------------------------------------------------------------
/// @brief Load an image bitmap from a file.
/// @param flip_vertically Flip image vertically.
/// @param n_channels load n pixel components (0 = load all components)
///
Image LoadImage(
    const std::string &filename,
    const bool flip_vertically,
    const int32_t n_channels)
{
    assert(!filename.empty());

    //
    // Load image data from the file using stb image loader:
    //  x = width
    //  y = height
    //  n = # 8-bit components per pixel
    //  '0' = load all available components
    //
    // The pixel data consists of h-scanlines with w-pixels each, and each pixel
    // consists of n-interleaved 8-bit components. The first pixel is at the
    // top-left-most in the image. There is no padding between image scanlines
    // or between pixels, regardless of the format. An output image with
    // n-components has the following interleaved order in each pixel:
    //  n = #comp      components
    //     1           grey
    //     2           grey, alpha
    //     3           red, green, blue
    //     4           red, green, blue, alpha
    //
    stbi_set_flip_vertically_on_load(flip_vertically ? 1 : 0);
    int w, h, n;
    uint8_t *data = stbi_load(filename.c_str(), &w, &h, &n, n_channels);
    assert(data != NULL);

    //
    // Create an image from the data. There is no padding between scanlines or
    // between pixels of a stb image, so we just use the image width to specify
    // its byte size.
    //
    uint32_t width = (uint32_t) w;
    uint32_t height = (uint32_t) h;
    uint32_t bpp = (uint32_t) (8 * (n_channels == 0 ? n : n_channels));

    Image image = CreateImage(width, height, bpp);

    uint32_t size = width * height * bpp / 8;
    std::memcpy(&image.bitmap[0], data, size);

    //
    // Free image data.
    //
    stbi_image_free(data);

    return image;
}

/// -----------------------------------------------------------------------------
/// @brief Save an image bitmap to a png file.
/// @param flip_vertically Flip image vertically.
/// @param n_channels load n pixel components (0 = load all components)
///
void SaveImagePng(
    const Image &image,
    const std::string &filename,
    const bool flip_vertically)
{
    assert(!filename.empty());

    // Function stbi_write_png returns 0 on failure and non-0 on success.
    stbi_flip_vertically_on_write(flip_vertically ? 1 : 0);
    int ret = stbi_write_png(
        filename.c_str(),           // name of the file
        image.width,                // image width
        image.height,               // image height
        (image.bpp >> 3),           // # components per pixel
        &image.bitmap[0],           // bitmap data
        image.pitch);               // scanline stride in bytes
    assert(ret != 0);
}

///
/// @brief Save an image bitmap using a plain PPM file format. The format has a
/// header followed by the image data. The header begins with a two-byte number
/// identifying the format of the image - P3 for plain ASCII and P6 for binary.
///
/// @par Plain PPM file layout. The layout of a plain PPM file is the same as
/// the binary one except for the following:
///  1. There is exactly one image in a file.
///  2. The magic number is P3 instead of P6.
///  3. Each sample in the raster is represented as an ASCII decimal number.
///  4. Each sample in the raster has white space before and after it.
///  5. No line should be longer than 70 characters.
///
/// @see http://paulbourke.net/dataformats/ppm
///      http://netpbm.sourceforge.net/doc/ppm.html
///      https://en.wikipedia.org/wiki/Netpbm_format
///
void SaveImagePpma(
    const Image &image,
    const std::string &filename,
    const bool flip_vertically)
{
    assert(!filename.empty());

    std::ostringstream buffer;

    // Write header.
    buffer << "P3\n"
           << image.width   << "#width\n"
           << image.height  << "#height\n"
           << 255           << "#colors\n";

    // Write bitmap data.
    for (uint32_t y = 0; y < image.height; ++y) {
        for (uint32_t x = 0; x < image.width; ++x) {
            const uint8_t *px = flip_vertically
                ? image(x, image.height - 1 - y)
                : image(x, y);

            uint8_t r = *px;
            uint8_t g = image.bpp > 8  ? *(px + 1) : 0;
            uint8_t b = image.bpp > 16 ? *(px + 2) : 0;

            buffer << static_cast<uint32_t>(r) << " "
                   << static_cast<uint32_t>(g) << " "
                   << static_cast<uint32_t>(b) << "\n";
        }
    }

    // Write buffer to PPM file.
    std::ofstream file(filename);
    if (file) {
        file << buffer.str();
    }
}

///
/// @brief Save an image bitmap using a binary PPM file format with layout:
///  1. Two-byte "magic number" P6 identifying the file format.
///  2. Whitespace (blanks, TABs, CRs, LFs).
///  3. Image width in pixel units (ASCII decimal).
///  4. Whitespace.
///  5. Image height in pixel units (ASCII decimal).
///  6. Whitespace.
///  7. Maximum color value (ASCII decimal).
///     Must be less than 65536 and more than zero.The RGB format is assumed to
///     be RGB888, i.e., 1 byte per color channel and a maximum value of 255.
///  8. Whitespace (usually a newline).
///  9. A raster of height rows, in order from top to bottom.
///     Each row consists of width pixels, in order from left to right.
///     Each pixel is a triplet of red, green, and blue samples, in that order.
///     Each sample is represented in pure binary by either 1 or 2 bytes.
///     If maximum color is less than 256, it is 1 byte. Otherwise,it is 2 bytes.
///     The most significant byte is first.
///
/// @see http://paulbourke.net/dataformats/ppm
///      http://netpbm.sourceforge.net/doc/ppm.html
///      https://en.wikipedia.org/wiki/Netpbm_format
///
void SaveImagePpmb(
    const Image &image,
    const std::string &filename,
    const bool flip_vertically)
{
    assert(!filename.empty());

    // Write header.
    std::ostringstream header;
    header << "P6\n"
           << image.width   << "#width\n"
           << image.height  << "#height\n"
           << 255           << "#colors\n";

    std::ofstream file(filename, std::ios::binary);
    if (file) {
        file.write(header.str().c_str(), header.str().size());
    }

    // Write bitmap data.
    for (uint32_t y = 0; y < image.height; ++y) {
        for (uint32_t x = 0; x < image.width; ++x) {
            const uint8_t *px = flip_vertically
                ? image(x, image.height - 1 - y)
                : image(x, y);

            uint8_t r = px[0];
            uint8_t g = image.bpp > 8  ? px[1] : 0;
            uint8_t b = image.bpp > 16 ? px[2] : 0;

            if (file) {
                std::array<uint8_t, 3> col = {r, g, b};
                file.write((char *) &col[0], 3 * sizeof(uint8_t));
            }
        }
    }
}

/// -----------------------------------------------------------------------------
/// @brief Generate an OpenGL 2d-texture from the specified image.
///
GLuint CreateTextureFromImage(Image &image)
{
    Texture2dCreateInfo info = {};
    info.width = image.width;
    info.height = image.height;
    info.internalformat = GL_RGBA8;
    info.pixelformat = image.format;
    info.pixeltype = GL_UNSIGNED_BYTE;
    info.pixels = static_cast<GLvoid *>(&image.bitmap[0]);
    return CreateTexture2d(info);
}

} // Graphics
