#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

FILE *readFile(char *fileName);
int calcNumOfPoint(FILE *file);
int calcNumOfCords(FILE *file);
double **fillDataPoints(FILE *file, int num_of_cords, int num_of_points);
double **readDataPointsFromFile(char *fileName);

int sanity_test();