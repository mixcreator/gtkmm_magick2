#pragma once

#include <Magick++.h>

namespace Image{

  // to image.h
  class Image{
  public:

      Magick::Image& getImage();

      bool Save(const std::string filename);

      bool Load(const std::string filename);

      bool Rotate(const double degree);

      bool Resize(const double zoom_in_factor);

  private:
      Magick::Image _image;

      bool _wasLoaded = false;
  };
}