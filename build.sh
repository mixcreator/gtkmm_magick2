#!/bin/sh

rm ./gtk+_app

g++ main.cpp \
    main_window.cpp \
    image.cpp \
    watermark.cpp \
    -o gtk+_app \
    `pkg-config gtkmm-3.0 --cflags --libs` -lboost_regex \
    -std=c++17 -Wall -Wextra -Wpedantic -pedantic \
    `Magick++-config --cxxflags --cppflags` \
    `Magick++-config --ldflags --libs`

