#include <Magick++.h>
#include <iostream>


#include "watermark.h"

namespace Watermark{

bool Watermark::Do(Magick::Image& image){
    try
    {
        if (!_wasLoaded){
            std::cout << "Image was not loaded !" << std::endl;
            return false;
        }

        _image.opacity(50000);
        _image.modulate(500.0, 100.0, 50.0);
        image.composite(_image, 
                        Magick::GravityType::CenterGravity,
                        Magick::CompositeOperator::BumpmapCompositeOp);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;          
}

bool Watermark::Init(const std::string filename){
    try{
        _image.read(filename);
        _wasLoaded = true;
    }
    catch(const std::exception &error_)
    {
        std::cout << "Caught exception: " << error_.what() << std::endl;
        _wasLoaded = false;
        return false;    
    }
    return true;
}

} // Watermark

