/**
 * @file  sfnum.h
 * @brief Header for the snailfish number tree module.
 */

#ifndef SFNUM_H
#define SFNUM_H

#include <stdio.h>

struct num *newNum(const char *str);
struct num *addNum(struct num *addend1, struct num *addend2);
void clearNum(struct num *numToClear);
int magnitudeNum(struct num *numToMag);
void printNum(struct num *numToPrint);

#endif /* SFNUM_H */