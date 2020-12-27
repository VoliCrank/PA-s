#include "fadeColorPicker.h"

fadeColorPicker::fadeColorPicker(double fade) { fadeFactor = fade; }

HSLAPixel fadeColorPicker::operator()(point p) {
    HSLAPixel pix = p.c.color;
    HSLAPixel result;
    result.h = pix.h;
    result.s = pix.s;
    int x = p.x - p.c.x;
    int y = p.y - p.c.y;
    double dist = sqrt(pow(x,2) + pow(y,2));
    result.l = pix.l * pow(fadeFactor, dist);
    return result;
}
