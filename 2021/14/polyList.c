#include "polyList.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct node {
  char val;
  struct node *next;
};

static struct node *head = NULL;
static struct node *tail = NULL;

static struct node *cursor = NULL;     // used to iterate over the list
static struct node *prevCursor = NULL; // trails one node behind cursor

// move the cursors one node further along the list
int cursorNext() {
  // move the cursors
  prevCursor = cursor;
  cursor = (cursor == NULL) ? NULL : cursor->next;
  // return cursor's value
  return (cursor == NULL) ? EOF : cursor->val;
}

// set the cursor to head
int cursorReset() {
  // set the cursor to head
  cursor = head;
  prevCursor = NULL;
  return (cursor == NULL) ? EOF : cursor->val;
}

// insert new node at the tail
void insertLast(char val) {
  // create a new node
  struct node *newNode = (struct node *)malloc(sizeof(struct node));
  newNode->val = val;
  newNode->next = NULL;

  if (head == NULL) {
    // if it's the only node, make it the head
    head = newNode;
  } else {
    // otherwhise make it the old tail's next
    tail->next = newNode;
  }
  // point tail to new node
  tail = newNode;
}

// insert new node inbetween cursor and prevCursor
int insertInCursorPair(char val) {
  if (cursor == NULL || prevCursor == NULL) {
    // not in a pair
    return 1;
  }
  // create a new node
  struct node *newNode = (struct node *)malloc(sizeof(struct node));
  newNode->val = val;

  // next node is the cursor
  newNode->next = cursor;
  // the node before this is the prevCursor
  prevCursor->next = newNode;
  // move the prevCursor forward
  prevCursor = newNode;

  return 0; // sucess
}

// determine the length of the list
int length() {
  int length = 0;
  struct node *i;
  for (i = head; i != NULL; i = i->next) {
    length++;
  }
  return length;
}

// count the occurences of the passed value in the list
int countValue(char val) {
  int counter = 0;
  struct node *i;
  for (i = head; i != NULL; i = i->next) {
    if (i->val == val) {
      counter++;
    }
  }
  return counter;
}

// deletes every node in the list
void clearPolyList() {
  // iterate through the list deleting every node
  cursorReset();
  while (cursorNext() != EOF) {
    if (prevCursor != NULL) {
      free(prevCursor);
    }
  }
  cursor = NULL;
  prevCursor = NULL;
}

// display the list
void printPolyList() {
  struct node *i = head;
  printf("[ ");
  while (i != NULL) {
    printf("%c ", i->val);
    i = i->next;
  }
  printf("]\n");
}