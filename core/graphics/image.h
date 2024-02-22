//
// image.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_IMAGE_H_
#define GRAPHICS_IMAGE_H_

#include <string>
#include <vector>
#include "core/graphics/common.h"

namespace Graphics {

///
/// @brief Image maintains a bitmap specified by:
///  Pixel storage layout:
///      width:  bitmap width in pixels.
///      height: bitmap height in pixels.
///      bpp:    bitmap pixel depth in bits per pixel.
///      stride: bitmap pixel stride in bytes per pixel,
///               4=32/8(RGBA), 3=24/8(RGB), 2=16/8(RG) and 1=8/8(R).
///  Bitmap size and data:
///      pitch:  bitmap row stride in bytes, calculated from its width
///              and bpp rounded to the next 4-byte (32-bit) boundary.
///      size:   size of the bitmap pixel buffer in bytes.
///      bitmap: bitmap pixel buffer data array.
///
struct Image {
    // Image data.
    uint32_t width;                 // bitmap width in pixels
    uint32_t height;                // bitmap height in pixels
    uint32_t bpp;                   // pixel bit depth
    GLenum format;                  // pixel format congruous with bit depth
    uint32_t pitch;                 // scanline size in bytes
    uint32_t size;                  // bitmap size in bytes, height*pitch
    std::vector<uint8_t> bitmap;    // bitmap pixel buffer array

    // Return a pointer to the image pixel (x,y).
    uint8_t *operator()(const uint32_t x, const uint32_t y);
    const uint8_t *operator()(const uint32_t, const uint32_t y) const;
};

///
/// @brief Access the pixel at row x and column y. If x and y are inside the
/// bitmap range, return a pointer to the pixel address specified by the pitch
/// size and number of colour components. Otherwise, return null.
///
inline uint8_t *Image::operator()(const uint32_t x, const uint32_t y)
{
    const uint32_t pixel_bytes = bpp >> 3;
    if (!bitmap.empty() && (x < width) && (y < height)) {
        return &bitmap[y * pitch + x * pixel_bytes];
    }
    return nullptr;
}

inline const uint8_t *Image::operator()(
    const uint32_t x, const uint32_t y) const
{
    const uint32_t pixel_bytes = bpp >> 3;
    if (!bitmap.empty() && (x < width) && (y < height)) {
        return &bitmap[y * pitch + x * pixel_bytes];
    }
    return nullptr;
}

///
/// @brief Return a string with image properties.
///
std::string GetImagePropertiesString(
    const Image &image,
    const char *comment = nullptr);

///
/// @brief Create an image with specified size and pixel depth.
///
Image CreateImage(
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp);

///
/// @brief Load/save an image.
///
Image LoadImage(
    const std::string &filename,
    const bool flip_vertically = false,
    const int32_t n_channels = 0);

void SaveImagePng(
    const Image &image,
    const std::string &filename,
    const bool flip_vertically = false);

void SaveImagePpma(
    const Image &image,
    const std::string &filename,
    const bool flip_vertically = false);

void SaveImagePpmb(
    const Image &image,
    const std::string &filename,
    const bool flip_vertically = false);

///
/// @brief Generate an OpenGL 2d-texture from the specified image.
///
GLuint CreateTextureFromImage(Image &image);

} // Graphics

#endif // GRAPHICS_IMAGE_H_
