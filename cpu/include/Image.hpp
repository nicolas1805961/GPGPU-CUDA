#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

class Image
{
private:
    int m_bpp;
    int m_height;
    int m_width;
    uint8_t *m_rgbImage;
    uint8_t *m_grayImage;

public:
    Image(std::string const& name);
    int getWidth() const;
    int getHeight() const;
    uint8_t *getImageRgb() const;
    uint8_t *getImageGray() const;
    ~Image();
};


