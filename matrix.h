#include "spkmeans.h"

/* given the initial data set contains n points */
double **createWeightedAdjMatrix(vector *initalDataPoints, int n);
double **createDegMatrix(double **weightedMatrix, int n);
double **createGraphLaplacian(double **weightedMatrix, double **degMatrix, int n);
double callDisBetweenToVector(cord *first_cord, cord *second_cord);
double calDeg(double **weightedMatrix, int i, int n);
double **createGraphLaplacian(double **weightedMatrix, double **degMatrix, int n);
void fillRowInWeightedAdjMatrix(double **weightedAdjMatrix, vector *head, vector *curent, int n, int i);