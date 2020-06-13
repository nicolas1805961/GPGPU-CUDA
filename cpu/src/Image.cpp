#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"
#include "Image.hpp"

Image::Image(std::string const& name)
{
    m_rgbImage = stbi_load(name.c_str(), &m_width, &m_height, &m_bpp, 3);
    m_grayImage = stbi_load(name.c_str(), &m_width, &m_height, &m_bpp, 1);
}

int Image::getWidth() const
{
    return m_width;
}

int Image::getHeight() const
{
    return m_height;
}

uint8_t* Image::getImageRgb() const
{
    return m_rgbImage;
}

uint8_t* Image::getImageGray() const
{
    return m_grayImage;
}

Image::~Image()
{
    stbi_image_free(m_rgbImage);
    stbi_image_free(m_grayImage);
}
