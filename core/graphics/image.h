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
#include <memory>
#include "common.h"
#include "texture.h"

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
struct ImageObject {
    // Image data.
    uint32_t mWidth;                 // bitmap width in pixels
    uint32_t mHeight;                // bitmap height in pixels
    uint32_t mBpp;                   // pixel bit depth
    GLenum mFormat;                  // pixel format congruous with bit depth
    uint32_t mPitch;                 // scanline size in bytes
    uint32_t mSize;                  // bitmap size in bytes, height*pitch
    std::vector<uint8_t> mBitmap;    // bitmap pixel buffer array

    // Return a pointer to the image pixel (x,y).
    uint8_t *Pixel(uint32_t x, uint32_t y);
    const uint8_t *Pixel(uint32_t x, uint32_t y) const;
};

using Image = std::unique_ptr<ImageObject>;

///
/// @brief Access the pixel at row x and column y. If x and y are inside the
/// bitmap range, return a pointer to the pixel address specified by the pitch
/// size and number of colour components. Otherwise, return null.
///
inline uint8_t *ImageObject::Pixel(uint32_t x, uint32_t y)
{
    const uint32_t pixelBytes = mBpp >> 3;
    if (!mBitmap.empty() && (x < mWidth) && (y < mHeight)) {
        return &mBitmap[y * mPitch + x * pixelBytes];
    }
    return nullptr;
}

inline const uint8_t *ImageObject::Pixel(uint32_t x, uint32_t y) const
{
    const uint32_t pixelBytes = mBpp >> 3;
    if (!mBitmap.empty() && (x < mWidth) && (y < mHeight)) {
        return &mBitmap[y * mPitch + x * pixelBytes];
    }
    return nullptr;
}

///
/// @brief Return a string with image properties.
///
std::string GetImageInfo(const Image &image, const char *comment = nullptr);

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
    const bool flipVertically = false,
    const int32_t nChannels = 0);

void SaveImagePng(
    const Image &image,
    const std::string &filename,
    const bool flipVertically = false);

void SaveImagePpma(
    const Image &image,
    const std::string &filename,
    const bool flipVertically = false);

void SaveImagePpmb(
    const Image &image,
    const std::string &filename,
    const bool flipVertically = false);

///
/// @brief Generate an OpenGL 2d-texture from the specified image.
///
Texture CreateTextureFromImage(Image &image);

} // Graphics

#endif // GRAPHICS_IMAGE_H_
