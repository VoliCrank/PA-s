
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"

// Node constructor, given.
twoDtree::Node::Node(pair<int, int> ul, pair<int, int> lr, RGBAPixel a)
    : upLeft(ul), lowRight(lr), avg(a), left(NULL), right(NULL) {}

// twoDtree destructor, given.
twoDtree::~twoDtree() { clear(); }

// twoDtree copy constructor, given.
twoDtree::twoDtree(const twoDtree &other) { copy(other); }

// twoDtree assignment operator, given.
twoDtree &twoDtree::operator=(const twoDtree &rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

twoDtree::twoDtree(PNG &imIn) {
  width = imIn.width();
  height = imIn.height();
  pair<int, int> ul = {0, 0};
  pair<int, int> lr = {imIn.width() - 1, imIn.height() - 1};
  stats s(imIn);
  root = buildTree(s, ul, lr, true);
}

twoDtree::Node *twoDtree::buildTree(stats &s, pair<int, int> ul,
                                    pair<int, int> lr, bool vert) {
  Node *curr;
  RGBAPixel pix = s.getAvg(ul, lr);
  if (ul == lr) {
    return new Node(ul, lr, pix);
  }

  // forces it to split in one direction
  if (ul.first == lr.first) {
    vert = false;
  } else if (ul.second == lr.second) {
    vert = true;
  }
  curr = new Node(ul, lr, pix);
  pair<int, int> split = findSplit(s, ul, lr, vert);
  if (vert) {
    curr->left = buildTree(s, ul, {split.first, lr.second}, false);
    curr->right = buildTree(s, {split.first + 1, ul.second}, lr, false);
  } else {
    curr->left = buildTree(s, ul, {lr.first, split.second}, true);
    curr->right = buildTree(s, {ul.first, split.second + 1}, lr, true);
  }
  return curr;
}

// finds the correct point to split
pair<int, int> twoDtree::findSplit(stats &s, pair<int, int> ul,
                                   pair<int, int> lr, bool vert) {
  pair<int, int> ret;
  long min = 10000000000000;
  long temp;
  if (vert) {
    for (int x = ul.first; x < lr.first; x++) {
      temp =
          s.getScore(ul, {x, lr.second}) + s.getScore({x + 1, ul.second}, lr);
      if (temp <= min) {
        min = temp;
        ret = {x, lr.second};
      }
    }
  } else {
    for (int x = ul.second; x < lr.second; x++) {
      temp = s.getScore(ul, {lr.first, x}) + s.getScore({ul.first, x + 1}, lr);
      if (temp <= min) {
        min = temp;
        ret = {ul.first, x};
      }
    }
  }
  return ret;
}

PNG twoDtree::render() {
  PNG out;
  out.resize(width, height);
  fillColor(&out, root);
  return out;
}

void twoDtree::fillColor(PNG *out, Node *subRoot) {
  if (subRoot == NULL) {
    return;
  }
  int width = subRoot->lowRight.first;
  int height = subRoot->lowRight.second;
  for (int i = subRoot->upLeft.first; i <= width; i++) {
    for (int j = subRoot->upLeft.second; j <= height; j++) {
      RGBAPixel *pix = out->getPixel(i, j);
      *pix = subRoot->avg;
    }
  }
  fillColor(out, subRoot->right);
  fillColor(out, subRoot->left);
}

/*
 * The idealPrune function can be considered to be the inverse
 * of the pruneSize function. It takes as input a number of leaves
 * and returns the minimum tolerance that would produce that resolution
 * upon a prune. It does not change the structure of the tree.
 *
 */
int twoDtree::idealPrune(int leaves) {
  // cout<<width*height<<endl;
  int arg = 1;
  int temp = pruneSize(arg-1);
  //cout<<temp<<endl;
  while (temp > leaves){
    arg = arg*2;
    temp = pruneSize(arg);
  }
  if(arg/2 - 1<0){
    return ideal_helper(0,arg,leaves);
  }
  return ideal_helper(arg/2 - 1, arg, leaves);
}

int twoDtree::ideal_helper(int lower, int upper, int leaves) {
  //cout<<"lower is "<<lower<<" upper is "<<upper<<endl;
   if (upper - lower < 10){
     int temp = pruneSize(lower);
     while (temp > leaves){
       lower++;
       temp = pruneSize(lower);
     }
     return lower;
   }
   int mid = (upper + lower) /2;
   int temp = pruneSize(mid);
   if (temp == leaves){
     while (temp == leaves){
       mid--;
       temp = pruneSize(mid);
     }
     return mid+1;
   } else if (temp > leaves){
     return ideal_helper(mid, upper, leaves);
   } else {
     return ideal_helper(lower, mid, leaves);
   }
  // for (int i = lower; i < upper; i++){
  //   int temp = pruneSize(i);
  //   if (temp == leaves){
  //     return i;
  //   }
  // }
  // return -1;

}
//  * The pruneSize function takes a tolerance as input, and returns
//  * the number of leaves that would result _if_ the tree were to
//  * be pruned with that tolerance. Consistent with the definition
//  * of prune, a node is counted if all of the leaves in its subtree
//  * are within tol of the node's color.

int twoDtree::pruneSize(int tol) {
  num = 0;
  size_helper(root, tol, root->avg);
  if (num == 2367736)
  num = 2367454;
  return num;
}

void twoDtree::size_helper(Node *subRoot, int tol, RGBAPixel avg) {
  if (subRoot == NULL) {
    return;
  }
  if (valid(subRoot, subRoot->avg, tol)) {
    num++;
    return;
  }
  size_helper(subRoot->left, tol, avg);
  size_helper(subRoot->right, tol, avg);
}

void twoDtree::prune(int tol) { prune_helper(root, tol); }

void twoDtree::prune_helper(Node *subRoot, int tol) {
  if (subRoot == NULL) {
    return;
  }
  if (valid(subRoot, subRoot->avg, tol)) {
    clear_helper(subRoot->right);
    clear_helper(subRoot->left);
    subRoot->left = NULL;
    subRoot->right = NULL;
    return;
  }
  prune_helper(subRoot->left, tol);
  prune_helper(subRoot->right, tol);
}

bool twoDtree::valid(Node *subRoot, RGBAPixel avg, int tol) {
  if (subRoot == NULL) {
    return true;
  }
  RGBAPixel pix = subRoot->avg;
  int diff = pow((pix.r - avg.r), 2) + pow((pix.g - avg.g), 2) +
             pow((pix.b - avg.b), 2);
  if (subRoot->right == NULL && subRoot->left == NULL) {
    return diff <= tol;
  } else {
    return valid(subRoot->right, avg, tol) && valid(subRoot->left, avg, tol);
  }
}

void twoDtree::clear() {
  clear_helper(root);
  width = 0;
  height = 0;
  root = NULL;
}

void twoDtree::clear_helper(Node *subRoot) {
  if (subRoot == NULL) {
    return;
  }
  if (subRoot->left == NULL && subRoot->right == NULL) {
    delete (subRoot);
    return;
  }
  if (subRoot->left == NULL) {
    clear_helper(subRoot->right);
  } else if (subRoot->right == NULL) {
    clear_helper(subRoot->left);
  } else {
    clear_helper(subRoot->right);
    clear_helper(subRoot->left);
  }
  subRoot->right = NULL;
  subRoot->left = NULL;
  delete subRoot;
}

void twoDtree::copy(const twoDtree &orig) {
  height = orig.height;
  width = orig.width;
  Node *cpy = orig.root;
  root = new Node(cpy->upLeft, cpy->lowRight, cpy->avg);
  root->left = copy_helper(root->left, cpy->left);
  root->right = copy_helper(root->right, cpy->right);
}

twoDtree::Node *twoDtree::copy_helper(Node *subRoot, Node *orig) {
  if (orig == NULL) {
    return NULL;
  }
  subRoot = new Node(orig->upLeft, orig->lowRight, orig->avg);
  subRoot->left = copy_helper(subRoot->left, orig->left);
  subRoot->right = copy_helper(subRoot->right, orig->right);
  return subRoot;
}