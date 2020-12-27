#include "solidColorPicker.h"

solidColorPicker::solidColorPicker(HSLAPixel fillColor) { color = fillColor; }

HSLAPixel solidColorPicker::operator()(point p) { return color; }
