#pragma once

#include <Magick++.h>

namespace Watermark{

    // to watermark.h
    class Watermark{
    public:
        bool Init(const std::string filename);
        bool wasLoaded() const{
            return _wasLoaded;
        }

        bool Do(Magick::Image& image);

    private:
        bool _wasLoaded=false;

        Magick::Image _image;
    };
} // Watermark
