#!/bin/sh

rm ./simple

g++ main.cpp -o simple `pkg-config gtkmm-3.0 --cflags --libs` -lboost_regex \
    -std=c++17 -Wall -Wextra -Wpedantic \
    `pkg-config gtkmm-3.0 --cflags --libs`  `Magick++-config --cxxflags --cppflags` \
    `Magick++-config --ldflags --libs`

