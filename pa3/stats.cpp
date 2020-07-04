#include "stats.h"

stats::stats(PNG &im) {
  sumRed.resize(im.width());
  sumGreen.resize(im.width());
  sumBlue.resize(im.width());
  sumsqRed.resize(im.width());
  sumsqGreen.resize(im.width());
  sumsqBlue.resize(im.width());
  for (unsigned long x = 0; x < im.width(); x++) {
    sumRed[x].resize(im.height());
    sumBlue[x].resize(im.height());
    sumGreen[x].resize(im.height());
    sumsqRed[x].resize(im.height());
    sumsqGreen[x].resize(im.height());
    sumsqBlue[x].resize(im.height());
    for (unsigned long y = 0; y < im.height(); y++) {
      RGBAPixel *pix = im.getPixel(x, y);
      if (x == 0 && y == 0) {
        sumRed[x][y] = pix->r;
        sumGreen[x][y] = pix->g;
        sumBlue[x][y] = pix->b;
        sumsqRed[x][y] = (pix->r) * (pix->r);
        sumsqBlue[x][y] = (pix->b) * (pix->b);
        sumsqGreen[x][y] = (pix->g) * (pix->g);
      } else if (x == 0) {
        sumRed[x][y] = pix->r + sumRed[x][y - 1];
        sumGreen[x][y] = pix->g + sumGreen[x][y - 1];
        sumBlue[x][y] = pix->b + sumBlue[x][y - 1];
        sumsqRed[x][y] = (pix->r) * (pix->r) + sumsqRed[x][y - 1];
        sumsqBlue[x][y] = (pix->b) * (pix->b) + sumsqBlue[x][y - 1];
        sumsqGreen[x][y] = (pix->g) * (pix->g) + sumsqGreen[x][y - 1];
      } else if (y == 0) {
        sumRed[x][y] = pix->r + sumRed[x - 1][y];
        sumGreen[x][y] = pix->g + sumGreen[x - 1][y];
        sumBlue[x][y] = pix->b + sumBlue[x - 1][y];
        sumsqRed[x][y] = (pix->r) * (pix->r) + sumsqRed[x - 1][y];
        sumsqBlue[x][y] = (pix->b) * (pix->b) + sumsqBlue[x - 1][y];
        sumsqGreen[x][y] = (pix->g) * (pix->g) + sumsqGreen[x - 1][y];
      } else {
        sumRed[x][y] =
            pix->r + sumRed[x - 1][y] + sumRed[x][y - 1] - sumRed[x - 1][y - 1];
        sumGreen[x][y] = pix->g + sumGreen[x - 1][y] + sumGreen[x][y - 1] -
                         sumGreen[x - 1][y - 1];
        sumBlue[x][y] = pix->b + sumBlue[x - 1][y] + sumBlue[x][y - 1] -
                        sumBlue[x - 1][y - 1];
        sumsqRed[x][y] = (pix->r) * (pix->r) + sumsqRed[x - 1][y] +
                         sumsqRed[x][y - 1] - sumsqRed[x - 1][y - 1];
        sumsqBlue[x][y] = (pix->b) * (pix->b) + sumsqBlue[x - 1][y] +
                          sumsqBlue[x][y - 1] - sumsqBlue[x - 1][y - 1];
        sumsqGreen[x][y] = (pix->g) * (pix->g) + sumsqGreen[x - 1][y] +
                           sumsqGreen[x][y - 1] - sumsqGreen[x - 1][y - 1];
      }
    }
  }
}

long stats::getSum(char channel, pair<int, int> ul, pair<int, int> lr) {
  long ret;
  if (channel == 'r') {
    if (ul.first == 0 && ul.second == 0) {
      ret = sumRed[lr.first][lr.second];
    } else if (ul.first == 0) {
      ret = sumRed[lr.first][lr.second] - sumRed[lr.first][ul.second - 1];
    } else if (ul.second == 0) {
      ret = sumRed[lr.first][lr.second] - sumRed[ul.first - 1][lr.second];
    } else {
      ret = sumRed[lr.first][lr.second] - sumRed[ul.first - 1][lr.second] -
            sumRed[lr.first][ul.second - 1] +
            sumRed[ul.first - 1][ul.second - 1];
    }
  } else if (channel == 'g') {
    if (ul.first == 0 && ul.second == 0) {
      ret = sumGreen[lr.first][lr.second];
    } else if (ul.first == 0) {
      ret = sumGreen[lr.first][lr.second] - sumGreen[lr.first][ul.second - 1];
    } else if (ul.second == 0) {
      ret = sumGreen[lr.first][lr.second] - sumGreen[ul.first - 1][lr.second];
    } else {
      ret = sumGreen[lr.first][lr.second] - sumGreen[ul.first - 1][lr.second] -
            sumGreen[lr.first][ul.second - 1] +
            sumGreen[ul.first - 1][ul.second - 1];
    }
  } else {
    if (ul.first == 0 && ul.second == 0) {
      ret = sumBlue[lr.first][lr.second];
    } else if (ul.first == 0) {
      ret = sumBlue[lr.first][lr.second] - sumBlue[lr.first][ul.second - 1];
    } else if (ul.second == 0) {
      ret = sumBlue[lr.first][lr.second] - sumBlue[ul.first - 1][lr.second];
    } else {
      ret = sumBlue[lr.first][lr.second] - sumBlue[ul.first - 1][lr.second] -
            sumBlue[lr.first][ul.second - 1] +
            sumBlue[ul.first - 1][ul.second - 1];
    }
  }
  return ret;
}

long stats::getSumSq(char channel, pair<int, int> ul, pair<int, int> lr) {
  long ret;
  if (channel == 'r') {
    if (ul.first == 0 && ul.second == 0) {
      ret = sumsqRed[lr.first][lr.second];
    } else if (ul.first == 0) {
      ret = sumsqRed[lr.first][lr.second] - sumsqRed[lr.first][ul.second - 1];
    } else if (ul.second == 0) {
      ret = sumsqRed[lr.first][lr.second] - sumsqRed[ul.first - 1][lr.second];
    } else {
      ret = sumsqRed[lr.first][lr.second] - sumsqRed[ul.first - 1][lr.second] -
            sumsqRed[lr.first][ul.second - 1] +
            sumsqRed[ul.first - 1][ul.second - 1];
    }
  } else if (channel == 'g') {
    if (ul.first == 0 && ul.second == 0) {
      ret = sumsqGreen[lr.first][lr.second];
    } else if (ul.first == 0) {
      ret =
          sumsqGreen[lr.first][lr.second] - sumsqGreen[lr.first][ul.second - 1];
    } else if (ul.second == 0) {
      ret =
          sumsqGreen[lr.first][lr.second] - sumsqGreen[ul.first - 1][lr.second];
    } else {
      ret = sumsqGreen[lr.first][lr.second] -
            sumsqGreen[ul.first - 1][lr.second] -
            sumsqGreen[lr.first][ul.second - 1] +
            sumsqGreen[ul.first - 1][ul.second - 1];
    }
  } else {
    if (ul.first == 0 && ul.second == 0) {
      ret = sumsqBlue[lr.first][lr.second];
    } else if (ul.first == 0) {
      ret = sumsqBlue[lr.first][lr.second] - sumsqBlue[lr.first][ul.second - 1];
    } else if (ul.second == 0) {
      ret = sumsqBlue[lr.first][lr.second] - sumsqBlue[ul.first - 1][lr.second];
    } else {
      ret = sumsqBlue[lr.first][lr.second] -
            sumsqBlue[ul.first - 1][lr.second] -
            sumsqBlue[lr.first][ul.second - 1] +
            sumsqBlue[ul.first - 1][ul.second - 1];
    }
  }
  return ret;
}

long stats::rectArea(pair<int, int> ul, pair<int, int> lr) {
  long x = lr.first - ul.first + 1;
  long y = lr.second - ul.second + 1;
  return x * y;
}

// given a rectangle, compute its sum of squared deviations from mean, over
// all color channels.
long stats::getScore(pair<int, int> ul, pair<int, int> lr) {
  long area = rectArea(ul, lr);
  long rval = getSum('r', ul, lr);
  long gval = getSum('g', ul, lr);
  long bval = getSum('b', ul, lr);
  long red = getSumSq('r', ul, lr);
  long blue = getSumSq('b', ul, lr);
  long green = getSumSq('g', ul, lr);
  long ret = red - rval * rval / area + green - gval * gval / area + blue -
             bval * bval / area;
  return ret;
}

RGBAPixel stats::getAvg(pair<int, int> ul, pair<int, int> lr) {
  long area = rectArea(ul, lr);
  long red = getSum('r', ul, lr) / area;
  long blue = getSum('b', ul, lr) / area;
  long green = getSum('g', ul, lr) / area;
  return RGBAPixel(red, green, blue);
}
