//
// Created by Alex Dong on 2020-01-22.
//
#include "block.h"

// set values in data attribute, based on a dimension x dimension square region
//   with upper-left corner at (left, upper) in the input PNG image
// The orientation of the pixels in the data vector must match the orientation
//   of the pixels in the PNG.
// PRE: upper and left (and upper + dimension, left + dimension) are valid
//        vector indices
void Block::Build(PNG &im, int upper, int left, int dimension) {

    data.resize(dimension);

    for (int i = 0; i < dimension; i++) {

        for (int j = 0; j <dimension; j++) {
            data[i].push_back(* im.getPixel(left + j, upper + i));
        }
    }
}

// write the pixel colour data fom data attribute into im,
//   with upper-left corner at (left, upper)
// PRE: upper and left (and upper + dimension, left + dimension) are valid
//        vector indices
void Block::Render(PNG &im, int upper, int left) const {
    int dimension = data.size();
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {

            *im.getPixel(left + j, upper + i) = data[i][j];
        }
    }
}

// "Reverse" the Hue and Luminance channels for each pixel in the data attribute
//   to simulate a photo-negative effect.
// Refer to the HSLAPixel documentation to determine an appropriate transformation
//   for "reversing" hue and luminance.
void Block::Negative() {
    int dimension = data.size();
    for (int i=0;i<dimension;i++){
        for (int j=0;j<dimension;j++){
            data[i][j].l = 1 - data[i][j].l;
            if (data[i][j].h + 180 >= 360){
                data[i][j].h = data[i][j].h -180;
            } else {
                data[i][j].h = data[i][j].h +180;
            }
        }
    }

}

// Return the horizontal (or vertical) size of the data block's image region
int Block::Dimension() const {
    return data[0].size();
}


