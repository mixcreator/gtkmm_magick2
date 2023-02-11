#include <Magick++.h>
#include <iostream>

#include "image.h"

namespace Image{

Magick::Image& Image::getImage() {
    return _image;
}


bool Image::Save(const std::string filename){
    try
    {
        if (!_wasLoaded){
            std::cout << "Image was not loaded !" << std::endl;
            return false;
        }

        _image.write(filename);
    }
    catch(const std::exception &error_)
    {
        std::cout << "Caught exception: " << error_.what() << std::endl;
        return false;
    }
    return true;
}

bool Image::Load(const std::string filename){
    try
    {
        _image.read(filename);
    }
    catch(const std::exception &error_)
    {
        std::cout << "Caught exception: " << error_.what() << std::endl;
        _wasLoaded = false;
        return false;    
    }

    _wasLoaded = true;

    return true;
} 


bool Image::Rotate(const double degree){

    if(!_wasLoaded)
    {
        std::cout << "Image was not loaded !" << std::endl;
        return false;
    }

    try
    {
        _image.rotate(degree);
    }
    catch(const std::exception &error_)
    {
        std::cout << "Caught exception: " << error_.what() << std::endl;
        return false;
    }

    return true;

}

bool Image::Resize(const double zoom_in_factor){

    if(!_wasLoaded)
    {
        std::cout << "Image was not loaded !" << std::endl;
        return false;
    }

    try
    {

        _image.zoom( Magick::Geometry(_image.columns()*zoom_in_factor, _image.rows()*zoom_in_factor));
        return true;
    }
    catch(const std::exception &error_)
    {
        std::cout << "Caught exception: " << error_.what() << std::endl;
        return false;    
    }

    return true;

}

bool Image::Resize(const std::string size)
{
    if(!_wasLoaded)
    {
        std::cout << "Image was not loaded !" << std::endl;
        return false;
    }

    try
    {

        _image.resize(size);
        return true;
    }
    catch(const std::exception &error_)
    {
        std::cout << "Caught exception: " << error_.what() << std::endl;
        return false;    
    }

    return true;    
}

}// Image namespace


