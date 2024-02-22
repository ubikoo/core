//
// main.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "core/graphics/graphics.h"

/// -----------------------------------------------------------------------------
/// @brief Constants and globals.
///
static const int kWidth = 800;
static const int kHeight = 800;
static const std::string kWriteDir = {"/tmp/"};
static const std::string kReadDir = {"../assets/"};
static const std::string kOutPrefix = {"out.image."};
static const std::vector<std::string> kImageFilenames = {
    "baboon_512.png",
    "color-wheel-80x80-blue.png",
    "color-wheel-80x80-bluea.png",
    "color-wheel-80x80-red.png",
    "color-wheel-80x80-reda.png",
    "color-wheel-80x80-rgb.png",
    "color-wheel-80x80-rgba.png",
    "color-wheel-rgb.png",
    "color-wheel-rgba.png",
    "equirectangular.png",
    "fruits_512.png",
    "monarch_512.png",
    "pool_512.png"};

/// -----------------------------------------------------------------------------
/// main test client
///
int main(int argc, char const *argv[])
{
    // ---- Test image factory functions ------------------------------------
    {
        Graphics::Image img8  = Graphics::CreateImage(kWidth, kHeight, 8);
        std::cout << Graphics::GetImagePropertiesString(img8, "bpp = 8") << "\n";

        Graphics::Image img16 = Graphics::CreateImage(kWidth, kHeight, 16);
        std::cout << Graphics::GetImagePropertiesString(img16, "bpp = 16") << "\n";

        Graphics::Image img24 = Graphics::CreateImage(kWidth, kHeight, 24);
        std::cout << Graphics::GetImagePropertiesString(img24, "bpp = 24") << "\n";

        Graphics::Image img32 = Graphics::CreateImage(kWidth, kHeight, 32);
        std::cout << Graphics::GetImagePropertiesString(img32, "bpp = 32") << "\n";
    }

    // ---- Test red images -------------------------------------------------
    {
        std::string filename("color-wheel-80x80-red.png");
        std::string out_png(kOutPrefix + "color-wheel-80x80-red.png");
        std::string out_ppma(kOutPrefix + "color-wheel-80x80-red_p3.ppm");
        std::string out_ppmb(kOutPrefix + "color-wheel-80x80-red_p6.ppm");

        Graphics::Image image = Graphics::LoadImage(kReadDir + filename);
        std::cout << Graphics::GetImagePropertiesString(image,
            "color-wheel-80x80-red") << "\n";
        Graphics::SaveImagePng(image, kWriteDir + out_png);
        Graphics::SaveImagePpma(image, kWriteDir + out_ppma);
        Graphics::SaveImagePpmb(image, kWriteDir + out_ppmb);
    }

    {
        std::string filename("color-wheel-80x80-reda.png");
        std::string out_png(kOutPrefix + "color-wheel-80x80-reda-reda.png");
        std::string out_ppma(kOutPrefix + "color-wheel-80x80-reda-reda_p3.ppm");
        std::string out_ppmb(kOutPrefix + "color-wheel-80x80-reda-reda_p6.ppm");

        Graphics::Image image = Graphics::LoadImage(kReadDir + filename);
        std::cout << Graphics::GetImagePropertiesString(image,
            "color-wheel-80x80-reda") << "\n";
        Graphics::SaveImagePng(image, kWriteDir + out_png);
        Graphics::SaveImagePpma(image, kWriteDir + out_ppma);
        Graphics::SaveImagePpmb(image, kWriteDir + out_ppmb);
    }

    // ---- Test blue images ------------------------------------------------
    {
        std::string filename("color-wheel-80x80-blue.png");
        std::string out_png(kOutPrefix + "color-wheel-80x80-blue.png");
        std::string out_ppma(kOutPrefix + "color-wheel-80x80-blue_p3.ppm");
        std::string out_ppmb(kOutPrefix + "color-wheel-80x80-blue_p6.ppm");

        Graphics::Image image = Graphics::LoadImage(kReadDir + filename);
        std::cout << Graphics::GetImagePropertiesString(image,
            "color-wheel-80x80-blue") << "\n";
        Graphics::SaveImagePng(image, kWriteDir + out_png);
        Graphics::SaveImagePpma(image, kWriteDir + out_ppma);
        Graphics::SaveImagePpmb(image, kWriteDir + out_ppmb);
    }

    {
        std::string filename("color-wheel-80x80-bluea.png");
        std::string out_png(kOutPrefix + "color-wheel-80x80-bluea-bluea.png");
        std::string out_ppma(kOutPrefix + "color-wheel-80x80-bluea-bluea_p3.ppm");
        std::string out_ppmb(kOutPrefix + "color-wheel-80x80-bluea-bluea_p6.ppm");

        Graphics::Image image = Graphics::LoadImage(kReadDir + filename);
        std::cout << Graphics::GetImagePropertiesString(image,
            "color-wheel-80x80-bluea") << "\n";
        Graphics::SaveImagePng(image, kWriteDir + out_png);
        Graphics::SaveImagePpma(image, kWriteDir + out_ppma);
        Graphics::SaveImagePpmb(image, kWriteDir + out_ppmb);
    }

    // ---- Test image load -------------------------------------------------
    {
        for (auto &filename : kImageFilenames) {
            std::string out_png = kOutPrefix + "" + filename + ".png";
            std::string out_ppma = kOutPrefix + "" + filename + "_p3.ppm";
            std::string out_ppmb = kOutPrefix + "" + filename + "_p6.ppm";

            Graphics::Image image = Graphics::LoadImage(kReadDir + filename);

            std::cout << Graphics::GetImagePropertiesString(image,
                filename.c_str()) << "\n";
            Graphics::SaveImagePng(image, kWriteDir + out_png);
            Graphics::SaveImagePpma(image, kWriteDir + out_ppma);
            Graphics::SaveImagePpmb(image, kWriteDir + out_ppmb);
        }
    }

    exit(EXIT_SUCCESS);
}
