#pragma once

#include <Magick++.h>

namespace Watermark{
    
class Watermark
{
public:
    Watermark() = default;

    bool Init(const std::string filename);

    bool wasLoaded() const{
        return _wasLoaded;
    }

    bool Do(Magick::Image& image);

private:
    bool _wasLoaded=false;

    Magick::Image _image;
};

} // namespace Watermark
