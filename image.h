#pragma once

#include <Magick++.h>

namespace Image{

class Image
{
public:
    Image() = default;

    Magick::Image& getImage();

    bool Save(const std::string filename);

    bool Load(const std::string filename);

    bool Rotate(const double degree);

    bool Resize(const double zoom_in_factor);

    bool Resize(const std::string size);

private:
    Magick::Image _image;

    bool _wasLoaded = false;
};

} // namespace Image