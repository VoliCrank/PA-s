/**
 * @file filler.cpp
 * Implementation of functions in the filler namespace.
 *
 */

/**
 * Performs a multi-point flood fill using breadth first search.
 *
 * @param config      FillerConfig struct to setup the fill
 * @return animation  object illustrating progression of flood fill algorithm
 */
animation filler::fillBFS(FillerConfig &config) {
  if (config.centers.size() != config.pickers.size()) {
    cout << "unmatched length in BFS";
  }
  return fill<Queue>(config);
}

/**
 * Performs a multi-point flood fill using depth first search.
 *
 * @param config      FillerConfig struct to setup the fill
 * @return animation  object illustrating progression of flood fill algorithm
 */
animation filler::fillDFS(FillerConfig &config) {
  if (config.centers.size() != config.pickers.size()) {
    cout << "unmatched length in DFS";
  }
  return fill<Stack>(config);
}

template <template <class T> class OrderingStructure>
animation filler::fill(FillerConfig &config) {
  animation anim;
  OrderingStructure<point> list;
  init(config);
  count = 0;
  anim.addFrame(img);
  for (int i = 0; i < config.centers.size(); i++) {
    center ctr = config.centers[i];
    colorPicker *picker = config.pickers[i];
    point p = point(ctr);
    HSLAPixel *pix = img.getPixel(p.c.x, p.c.y);
    updateAndStore(picker, pix, p);
    animate(anim);
    list.add(p);
    fillOne(picker, list, anim);
  }
  anim.addFrame(img);
  visited.resize(0, 0);
  return anim;
}

void filler::init(FillerConfig &config) {
  img = config.img;
  tolerance = config.tolerance;
  freq = config.frameFreq;
  visited.resize(img.width(), img.height());
}

void filler::fillOne(colorPicker *picker, OrderingStructure<point> &list,
                     animation &anim) {

  while (!list.isEmpty()) {
    point p = list.remove();
    point left = point(p.x - 1, p.y, p.c);
    point down = point(p.x, p.y + 1, p.c);
    point right = point(p.x + 1, p.y, p.c);
    point up = point(p.x, p.y - 1, p.c);
    if (checkValid(left, picker)) {
      list.add(left);
      animate(anim);
    }
    if (checkValid(down, picker)) {
      list.add(down);
      animate(anim);
    }
    if (checkValid(right, picker)) {
      list.add(right);
      animate(anim);
    }
    if (checkValid(up, picker)) {
      list.add(up);
      animate(anim);
    }
  }
}

bool filler::checkValid(point p, colorPicker *picker) {
  int x = p.x;
  int y = p.y;
  bool valid = x >= 0 && y >= 0 && x < img.width() && y < img.height();
  if (!valid || contains(p)) {
    return false;
  }
  if (valid) {
    HSLAPixel ctr = p.c.color;
    HSLAPixel *pix = img.getPixel(x, y);
    if (pix->dist(ctr) <= tolerance) {
      updateAndStore(picker, pix, p);
      return true;
    }
  }
  return false;
}

void filler::updateAndStore(colorPicker *picker, HSLAPixel *pix, point p) {
  *pix = picker->operator()(p);
  HSLAPixel *visit = visited.getPixel(p.x, p.y);
  visit->a = 0;
  count++;
}

void filler::animate(animation &anim) {
  if (count % freq == 0) {
    anim.addFrame(img);
  }
}

bool filler::contains(point p) {
  HSLAPixel *visit = visited.getPixel(p.x, p.y);
  return visit->a == 0;
}
/**
 * Run a multi-point flood fill on an image
 *
 * @param  config     FillerConfig struct with data for flood fill of image
 * @return animation  object illustrating progression of flood fill algorithm
 */
