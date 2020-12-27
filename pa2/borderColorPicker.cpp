#include "borderColorPicker.h"

borderColorPicker::borderColorPicker(unsigned int borderSize,
                                     HSLAPixel fillColor, PNG &img,
                                     double tolerance) {
  this->borderSize = borderSize;
  this->fillColor = fillColor;
  this->tolerance = tolerance;
  this->img = img;
}

HSLAPixel borderColorPicker::operator()(point p) {
  HSLAPixel *ret = img.getPixel(p.x, p.y);
  if (checkBoundaryPixels(p)) {
    return fillColor;
  }
  return *ret;
}

bool borderColorPicker::checkBoundaryPixels(point p) {
  for (int i = 0 ;i <= borderSize; i++) {
    for (int j = 0; j <= borderSize; j++) {
      if (i * i + j * j <= borderSize * borderSize) {
        if (checkUpDown(p, i, j)) {
          return true;
        }
      }
    }
  }
  return false;
}

bool borderColorPicker::checkUpDown(point p, int i, int j) {
  int x = p.x + i;
  int y = p.y + j;
  int x1 = p.x - i;
  int y1 = p.y - j;
  HSLAPixel ctr = p.c.color;
  return checkOne(x, y, ctr) || checkOne(x1, y1, ctr)||checkOne(x1,y,ctr) ||checkOne(x,y1,ctr);
}

bool borderColorPicker::checkOne(int x, int y, HSLAPixel ctr) {
  bool valid = x >= 0 && y >= 0 && x < img.width() && y < img.height();
  if (valid){
    HSLAPixel *pix = img.getPixel(x,y);
    if(pix->dist(ctr) <= tolerance){
      return false;
    }
  }
  return true;
}

