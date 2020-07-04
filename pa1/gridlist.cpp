// File:        gridlist.cpp
// Date:        2020-01-18 00:11
// Description: Contains partial implementation of GridList class
//              CPSC 221 2019W2 PA1
//              Function bodies to be completed by yourselves
//
// ADD YOUR FUNCTION IMPLEMENTATIONS IN THIS FILE
//

#include "gridlist.h"
#include "gridlist_given.cpp"

// Creates a PNG of appropriate pixel dimensions according to the GridList's
// structure
//   and colours each pixel according the each GridNode's Block data.
// The fully coloured PNG is returned.
PNG GridList::Render() const {
  PNG image;
  GridNode *curr = northwest;
  if (curr != NULL) {
    int blockdim = curr->data.Dimension();
    image.resize(blockdim * dimx, blockdim * dimy);
    for (int block_y = 0; block_y < dimy; block_y++) {
      for (int block_x = 0; block_x < dimx; block_x++) {
        curr->data.Render(image, block_y * blockdim, block_x * blockdim);
        curr = curr->next;
      }
    }
  }
  return image;
}

// Allocates a new GridNode containing bdata and
//   attaches it to end of this list.
// Be careful of the special case of inserting into an empty list.
void GridList::InsertBack(const Block &bdata) {

  GridNode *b = new GridNode(bdata);
  b->next = NULL;
  if (northwest != NULL && southeast != NULL) {
    southeast->next = b;
    b->prev = southeast;
    southeast = southeast->next;
  } else {
    northwest = b;
    southeast = b;
  }
}

// if this list has an odd number of column blocks, then the right side will
// have more blocks PRE:  this list and inner are not the same physical list
//       this list must have at least two column blocks
//       inner list must have at least one column block
//       inner list must have the same vertical resolution, vertical block
//       dimension, and block size
// POST: this list has nodes of the entire inner list in between the left and
// right halves of this list
//         and inner list becomes empty
//       otherwise both lists are not modified
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::Sandwich_H(GridList &inner) {
  if (northwest != inner.northwest && inner.dimy == dimy) {
    MergeH(inner);
    dimx = inner.dimx + dimx;
    inner.northwest = NULL;
    inner.southeast = NULL;
  }
}

// inner list must have the same horizontal resolution, horizontal block
// dimension, and block size if this list has an odd number of row blocks, then
// the bottom side will have more blocks PRE:  this list and inner are not the
// same physical list
//       this list must have at least two row blocks
//       inner list must have at least one row block
//       inner list must have same horizontal resolution, horizontal block
//       dimension, and block size
// POST: this list has nodes of the entire inner list in between the upper and
// lower halves of this list
//         and inner list becomes empty
//       otherwise both lists are not modified
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::Sandwich_V(GridList &inner) {
  GridNode *curr = northwest;
  GridNode *in = inner.northwest;
  if (in != curr && dimx == inner.dimx) {
    for (int y = 0; y < dimy / 2; y++) {
      for (int x = 0; x < dimx; x++) {
        curr = curr->next;
      }
    }
    curr = curr->prev;
    GridNode *temp = curr->next;
    for (int y = 0; y < inner.dimy; y++) {
      for (int x = 0; x < dimx; x++) {
        curr->next = in;
        in->prev = curr;
        curr = curr->next;
        in = in->next;
      }
    }
    curr->next = temp;
    temp->prev = curr;
    dimy = dimy + inner.dimy;
    inner.northwest = NULL;
    inner.southeast = NULL;
    inner.dimx = 0;
    inner.dimy = 0;
  }
}

// PRE:  this list and otherlist have the same pixel dimensions, block
// dimensions, and block size POST: this list and otherlist are checkered with
// each other's nodes
//       each list's dimensions remain the same
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::CheckerSwap(GridList &otherlist) {
  if (dimx == otherlist.dimx && dimy == otherlist.dimy) {
    GridNode *curr = northwest;
    GridNode *other = otherlist.northwest;
    if (dimx % 2 != 0) {
      for (int y = 0; y < dimy * dimx; y++) {
        CheckSwapShuffle(curr, other);
        curr = curr->next;
        other = other->next;
      }

    } else {
      for (int y = 0; y < dimy; y++) {
        for (int x = 0; x < dimx - 1; x++) {
          CheckSwapShuffle(curr, other);
          curr = curr->next;
          other = other->next;
        }
        curr = curr->next;
        other = other->next;
      }
    }
    other = otherlist.northwest;
    ReLink(other);
  }
}

// POST: this list has the negative effect applied selectively to GridNodes to
// form
//         a checkerboard pattern.
//       The northwest block is normal (does not have the negative effect
//       applied).
// Ensure that the checkering looks correct for both odd and even horizontal
// block dimensions
void GridList::CheckerN() {
  GridNode *curr = northwest;
  if (dimx % 2 != 0) {
    OddCheck(true, curr->next);
  } else {
    EvenCheck(false, curr);
  }
}

// Deallocates any dynamic memory associated with this list
//   and re-initializes this list to an empty state
void GridList::Clear() {
  GridNode *curr = northwest;
  while (curr != NULL && curr->next != NULL) {
    curr = curr->next;
    delete curr->prev;
  }
  delete curr;
  northwest = NULL;
  southeast = NULL;
  dimx = 0;
  dimy = 0;
}

// Allocates new nodes into this list as copies of all nodes from otherlist
void GridList::Copy(const GridList &otherlist) {
  if (otherlist.northwest != NULL) {
    northwest = new GridNode(otherlist.northwest->data);
    GridNode *other = otherlist.northwest;
    GridNode *curr = northwest;
    other = other->next;
    while (other != NULL) {
      GridNode *next = new GridNode(other->data);
      curr->next = next;
      next->prev = curr;
      curr = curr->next;
      other = other->next;
    }
    dimx = otherlist.dimx;
    dimy = otherlist.dimy;
  }
}

// IMPLEMENT ANY PRIVATE FUNCTIONS YOU HAVE ADDED BELOW
//
//

// Loops through the outer list and merging inner into the center in Sandwich_H
void GridList ::MergeH(GridList &inner) {
  GridNode *in = inner.northwest;
  GridNode *curr = northwest;
  int split = dimx / 2 - 1;
  for (int y = 0; y < dimy; y++) {
    for (int x = 0; x < dimx; x++) {
      if (x == split) {
        GridNode *temp = curr->next;
        for (int i = 0; i < inner.dimx; i++) {
          curr->next = in;
          in->prev = curr;
          curr = curr->next;
          in = in->next;
        }
        temp->prev = curr;
        curr->next = temp;
      }
      curr = curr->next;
    }
  }
}

// makes gridlist negative if it has odd width by Negative(ing) every other
// block
void GridList ::OddCheck(bool neg, GridNode *curr) {
  if (curr == NULL) {

  } else {
    if (neg) {
      Block b = curr->data;
      b.Negative();
      curr->data = b;
    }
    OddCheck(!neg, curr->next);
  }
}

// Makes gridlist negative while accounting for even width, alternating starts
void GridList::EvenCheck(bool check, GridNode *curr) {
  for (int y = 0; y < dimy; y++) {
    for (int x = 0; x < dimx; x++) {
      if (check) {
        Block b = curr->data;
        b.Negative();
        curr->data = b;
      }
      curr = curr->next;
      check = !check;
    }
    check = !check;
  }
}

// Goes to the beginning of the list and sets the prev of each element
void GridList::ReLink(GridNode *other) {
  GridNode *curr = northwest;
  while (curr->next != NULL) {
    GridNode *temp = curr->next;
    temp->prev = curr;
    GridNode *othertemp = other->next;
    othertemp->prev = other;
    curr = curr->next;
    other = other->next;
  }
}

// Points next node of curr to next node of other and vice versa
void GridList::CheckSwapShuffle(GridNode *curr, GridNode *other) {
  GridNode *temp;
  GridNode *othertemp;
  temp = curr->next;
  othertemp = other->next;
  curr->next = othertemp;
  other->next = temp;
}