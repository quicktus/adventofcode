/**
 * @file  polyList.h
 * @brief Header for the polymer list module.
 */

#ifndef POLYLIST_H
#define POLYLIST_H

#include <stdio.h>

int cursorNext();
int cursorReset();
void insertLast(char val);
int insertInCursorPair(char val);
int length();
int countValue(char val);
void clearPolyList();
void printPolyList();

#endif /* POLYLIST_H */