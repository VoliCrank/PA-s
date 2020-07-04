#define CATCH_CONFIG_MAIN
#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "cs221util/catch.hpp"
#include "stats.h"
#include "twoDtree.h"
#include <iostream>
#include <sys/stat.h>
#include <vector>

using namespace std;
using namespace cs221util;

// TEST_CASE("stats::basic rectArea", "[weight=1][part=stats]") {

//   PNG data;
//   data.resize(2, 3);

//   stats s(data);
//   pair<int, int> ul(0, 0);
//   pair<int, int> lr(1, 2);
//   long result = s.rectArea(ul, lr);

//   REQUIRE(result == 6);
// }

// TEST_CASE("stats::basic getAvg", "[weight=1][part=stats]") {
//   PNG data;
//   data.resize(3, 2);
//   for (int i = 0; i < 2; i++) {
//     for (int j = 0; j < 2; j++) {
//       RGBAPixel *p = data.getPixel(i, j);
//       p->r = 20 * i + 3 * j;
//       p->g = 3 * i + 20 * j;
//       p->b = 23 * i + 23 * j;
//       p->a = 1.0;
//     }
//   }
//   stats s(data);
//   pair<int, int> ul(1, 1);
//   pair<int, int> lr(1, 1);
//   RGBAPixel result = s.getAvg(ul, lr);
//   RGBAPixel expected(23, 23, 46);
//   REQUIRE(result.g == expected.g);
//   REQUIRE(result.b == expected.b);
//   REQUIRE(result.r == expected.r);

//   REQUIRE(result == expected);
// }

// TEST_CASE("stats::basic variance", "[weight=1][part=stats]") {
//   PNG data;
//   data.resize(3, 3);
//   for (int i = 0; i < 3; i++) {
//     for (int j = 0; j < 3; j++) {
//       RGBAPixel *p = data.getPixel(i, j);
//       p->r = 20 * i + 3 * j;
//       p->g = 3 * i + 20 * j;
//       p->b = 23 * i + 23 * j;
//       p->a = 1.0;
//     }
//   }
//   stats s(data);
//   pair<int, int> ul(0, 0);
//   pair<int, int> lr(1, 1);
//   long result = s.getScore(ul, lr);

//   REQUIRE(result == 1876);
// }

// TEST_CASE("twoDtree::basic ctor render", "[weight=1][part=twoDtree]") {
//   PNG img;
//   img.readFromFile("images/geo.png");

//   twoDtree t1(img);
// //t1.prune(5000);
//   PNG out = t1.render();
//    out.writeToFile("out/geo.png");
//   REQUIRE(out == img);
// }

// TEST_CASE("twoDtree::basic copy", "[weight=1][part=twoDtree]") {
//   PNG img;
//   img.readFromFile("images/geo.png");

//   twoDtree t1(img);
//   twoDtree t1copy(t1);

//   PNG out = t1copy.render();

//   REQUIRE(out == img);
// }

// TEST_CASE("twoDtree::basic prune", "[weight=1][part=twoDtree]") {
//   PNG img;
//   img.readFromFile("images/ubc-totem-poles.png");

//   twoDtree t1(img);
//   t1.prune(10000);

//   PNG result = t1.render();
//   stats s(img);
//   PNG expected;
//   expected.readFromFile("images/output-prunedubctotem.png");
//   result.writeToFile("out/soln.png");

//   REQUIRE(expected == result);
// }

TEST_CASE("twoDtree::basic pruneSize", "[weight=1][part=twoDtree]") {
  PNG img;
  img.readFromFile("images/ubc-totem-poles.png");

  twoDtree t1(img);
  int result = t1.pruneSize(3000);

  int expected = 13904;

  REQUIRE(expected == result);
}

TEST_CASE("twoDtree::basic idealPrune","[weight=1][part=twoDtree]"){
    PNG img;
    img.readFromFile("images/ada.png");

    twoDtree t1(img);
    int result = t1.idealPrune(13904);

    int expected = 2998;

    REQUIRE(expected==result);
}

TEST_CASE("twoDtree::basic idealPrune 1", "[weight=1][part=twoDtree]") {
  PNG img;
  img.readFromFile("images/ubc-totem-poles.png");
  twoDtree t1(img);
  int temp = t1.pruneSize(0);
  REQUIRE(0 == t1.idealPrune(temp));
  t1.prune(0);
}
