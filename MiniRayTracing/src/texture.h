#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec3.h"
#include "utils.h"


class Texture 
{
public:
    virtual Color value(double u, double v, const Point3& p) const = 0;
};

class SolidColor: public Texture 
{
public:
    SolidColor() {}
    SolidColor(Color c) : colorValue(c) {}

    SolidColor(double red, double green, double blue)
        : SolidColor(Color(red, green, blue)) {}

    virtual Color value(double u, double v, const Vec3& p) const override 
    {
        return colorValue;
    }

private:
    Color colorValue;
};


class CheckherTexture : public Texture 
{
public:
    CheckherTexture() {}

    CheckherTexture(shared_ptr<Texture> _even, shared_ptr<Texture> _odd)
        : even(_even), odd(_odd) {}

    CheckherTexture(Color c1, Color c2)
        : even(make_shared<SolidColor>(c1)), odd(make_shared<SolidColor>(c2)) {}

    virtual Color value(double u, double v, const Point3& p) const override 
    {
        auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

public:
    shared_ptr<Texture> odd;
    shared_ptr<Texture> even;
};


class ImageTexture : public Texture
{
public:
    const static int bytes_per_pixel = 3;

    ImageTexture() 
        : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

    ImageTexture(const char* filename)
    {
        int component_per_pixel = bytes_per_pixel;
        data = stbi_load(filename, &width, &height, &component_per_pixel, component_per_pixel);

        if (!data)
        {
            std::cerr << "ERROR: Image texture couldnt not loaded : " << filename << std::endl;
            width = height = 0;
        }

        bytes_per_scanline = width * bytes_per_pixel;
    }

    ~ImageTexture()
    {
        delete data;
    }

    virtual Color value(double u, double v, const Vec3& p) const override
    {
        if (!data)
        {
            return Color(0, 1, 1);
        }

        u = clamp(u, 0.0, 1.0);
        v = 1 - clamp(v, 0.0, 1.0);

        int i = static_cast<int>(u * (width - 1));
        int j = static_cast<int>(v * (height - 1));

        const auto color_scale = 1.0 / 255.0;
        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

private:
    unsigned char* data;
    int width;
    int height;
    int bytes_per_scanline;
};

#endif