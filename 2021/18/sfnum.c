#include "sfnum.h"

#include <stdio.h>
#include <stdlib.h>

static struct num *newLeafNum(struct num *parent, const char *str, int *idx);
static int reduceNum(struct num *numToReduce, int depth);
static void explodeNum(struct num *numToExplode, int idx);
static struct num *splitNum(struct num *numToSplit, int idx);

struct num {
  int leftVal;
  int rightVal;
  struct num *parent;
  struct num *left;
  struct num *right;
};

struct num *newNum(const char *str) {
  int idx = 1;
  // Allocate memory for root num
  struct num *num = (struct num *)malloc(sizeof(struct num));
  // see if the first element is a nested list
  if (str[idx] == '[') {
    idx++;
    num->leftVal = -1;
    num->left = newLeafNum(num, str, &idx);
  } else {
    // Assign value
    num->leftVal = (int)str[idx] - '0';
    num->left = NULL;
  }
  // skip comma
  idx += 2;
  // see if the second element is a nested list
  if (str[idx] == '[') {
    idx++;
    num->rightVal = -1;
    num->right = newLeafNum(num, str, &idx);
  } else {
    // Assign value
    num->rightVal = (int)str[idx] - '0';
    num->right = NULL;
  }

  // Root has no parent
  num->parent = NULL;
  return (num);
}

static struct num *newLeafNum(struct num *parent, const char *str, int *idx) {
  // Allocate memory for leaf num
  struct num *num = (struct num *)malloc(sizeof(struct num));
  // see if the first element is a nested list
  if (str[(*idx)] == '[') {
    (*idx)++;
    num->leftVal = -1;
    num->left = newLeafNum(num, str, idx);
  } else {
    // Assign value
    num->leftVal = (int)str[(*idx)] - '0';
    num->left = NULL;
  }
  // skip comma
  (*idx) += 2;
  // see if the second element is a nested list
  if (str[(*idx)] == '[') {
    (*idx)++;
    num->rightVal = -1;
    num->right = newLeafNum(num, str, idx);
  } else {
    // Assign value
    num->rightVal = (int)str[(*idx)] - '0';
    num->right = NULL;
  }
  // skip closing bracket
  (*idx)++;
  // Set the leafs parent
  num->parent = parent;
  return (num);
}

struct num *addNum(struct num *addend1, struct num *addend2) {
  // Allocate memory for the new root num
  struct num *root = (struct num *)malloc(sizeof(struct num));
  // set the values
  root->leftVal = -1;
  root->rightVal = -1;
  root->parent = NULL;
  root->left = addend1;
  root->right = addend2;
  // set the root as the addents' parent
  addend1->parent = root;
  addend2->parent = root;
  // perform reduction until its integrity is restored
  int i = 1;
  printf("\nafter addition: ");
  printNum(root);
  fflush(stdout);
  while (i) {
    i = reduceNum(root, -1);
    if (i) {
      printNum(root);
      fflush(stdout);
    }
  }
  printf("\n\n");
  // return the resulting num
  return root;
}

void clearNum(struct num *numToClear) {
  // if it has leaves, free those first
  if (numToClear->leftVal != -1) {
    clearNum(numToClear->left);
  }
  if (numToClear->rightVal != -1) {
    clearNum(numToClear->right);
  }
  // then free the parents
  free(numToClear);
}

int magnitudeNum(struct num *numToMag) {
  // The magnitude of a pair is 3 times the magnitude of its left element plus 2
  // times the magnitude of its right element. The magnitude of a regular number
  // is just that number.
  int leftMag = 0;
  int rightMag = 0;

  if (numToMag->leftVal == -1) {
    leftMag = magnitudeNum(numToMag->left);
  } else {
    leftMag = numToMag->leftVal * 3;
  }
  if (numToMag->rightVal == -1) {
    rightMag = magnitudeNum(numToMag->right);
  } else {
    rightMag = numToMag->rightVal * 2;
  }
  return leftMag + rightMag;
}

void printNum(struct num *numToPrint) {
  printf("[");
  if (numToPrint->leftVal == -1) {
    printNum(numToPrint->left);
  } else {
    printf("%i", numToPrint->leftVal);
  }
  printf(",");
  if (numToPrint->rightVal == -1) {
    printNum(numToPrint->right);
  } else {
    printf("%i", numToPrint->rightVal);
  }
  printf("]");
}

static int reduceNum(struct num *numToReduce, int depth) {
  depth++;
  // traverse the tree
  // left
  if (numToReduce->leftVal == -1) {
    // there is a nested num
    if (reduceNum(numToReduce->left, depth)) {
      return 1;
    }
  } else {
    // this is a regular number
    // check for explode criterion
    if (depth >= 4) {
      explodeNum(numToReduce, 0);
      printf("\nafter explode:  ");
      fflush(stdout);
      return 1;
    }
    // check for split criterion
    if (numToReduce->leftVal >= 10) {
      numToReduce->left = splitNum(numToReduce, 0);
      numToReduce->leftVal = -1;
      printf("\nafter split:    ");
      fflush(stdout);
      return 1;
    }
  }
  // right
  if (numToReduce->rightVal == -1) {
    // there is a nested num
    if (reduceNum(numToReduce->right, depth)) {
      return 1;
    }
  } else {
    // this is a regular number
    // check for explode criterion
    if (depth >= 4) {
      explodeNum(numToReduce, 1);
      printf("\nafter explode:  ");
      fflush(stdout);
      return 1;
    }
    // check for split criterion
    if (numToReduce->rightVal >= 10) {
      numToReduce->right = splitNum(numToReduce, 1);
      numToReduce->rightVal = -1;
      printf("\nafter split:    ");
      fflush(stdout);
      return 1;
    }
  }
  // no action needed to be performed - integrity is confirmed
  return 0;
}

static void explodeNum(struct num *numToExplode, int idx) {
  // Exploding pairs always consist of two regular numbers
  struct num *currentNum;
  struct num *previousNum;
  // the pair's left value is added to the first regular number to the left of
  // the exploding pair (if any)
  currentNum = numToExplode;
  previousNum = NULL;
  while (currentNum != NULL) {
    // this is the root or a lower num
    if (currentNum->left == previousNum) {
      // this num only has values further to the right
      // go higher
      previousNum = currentNum;
      currentNum = currentNum->parent;
    } else if (currentNum->leftVal != -1) {
      // found it - increase its value accordingly
      printf("\n\n\nleft: %i", currentNum->rightVal);
      currentNum->leftVal += numToExplode->leftVal;
      break;
    } else {
      // this num has values further down to the left
      // go down to the left
      currentNum = currentNum->left;
      // go to the rightmost of those left numbers
      while (currentNum->rightVal == -1) {
        currentNum = currentNum->right;
      }
      // found it - increase its value accordingly
      printf("\n\n\nleft: %i", currentNum->rightVal);
      currentNum->rightVal += numToExplode->leftVal;
      break;
    }
  }
  // the pair's right value is added to the first regular number to the right of
  // the exploding pair (if any)
  currentNum = numToExplode;
  previousNum = NULL;
  while (currentNum != NULL) {
    // this is the root or a lower num
    if (currentNum->right == previousNum) {
      // this num only has values further to the left
      // go higher
      previousNum = currentNum;
      currentNum = currentNum->parent;
    } else if (currentNum->rightVal != -1) {
      // found it - increase its value accordingly
      printf(" right: %i", currentNum->leftVal);
      currentNum->rightVal += numToExplode->rightVal;
      break;
    } else {
      // this num has values further down to the right
      // go down to the right
      currentNum = currentNum->right;
      // go to the leftmost of those right numbers
      while (currentNum->leftVal == -1) {
        currentNum = currentNum->left;
      }
    }
    // found it - increase its value accordingly
    printf(" right: %i", currentNum->leftVal);
    currentNum->leftVal += numToExplode->rightVal;
    break;
  }

  // the entire exploding pair is replaced with the regular number 0.
  if (!idx) {
    // left
    numToExplode->parent->leftVal = 0;
    numToExplode->parent->left = NULL;
  } else {
    // right
    numToExplode->parent->rightVal = 0;
    numToExplode->parent->right = NULL;
  }
  free(numToExplode);
}

static struct num *splitNum(struct num *numToSplit, int idx) {
  // Allocate memory for the new num
  struct num *splited = (struct num *)malloc(sizeof(struct num));
  int value = (idx) ? numToSplit->rightVal : numToSplit->leftVal;
  // set the values
  splited->leftVal = value / 2;        // divided by two and rounded down
  splited->rightVal = (value + 1) / 2; // divided by two and rounded up
  splited->parent = numToSplit;
  splited->left = NULL;
  splited->right = NULL;
  // return the splitted num
  return splited;
}
