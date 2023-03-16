#include <spkmeans.h>
#include <stdio.h>
#include <math.h>

double **createWeightedAdjMatrix(double **data_points, int num_of_point,int num_of_cord)
{
    double **weightedAdjMatrix = caluc(num_of_point, sizeof(double*));
    int i;
    for (i = 0; i < num_of_point; i++)
    {
        fillRowInWeightedAdjMatrix(weightedAdjMatrix,data_points,num_of_cord,num_of_point, i);
        curent = curent->next;
    }
    return weightedAdjMatrix;
}
void fillRowInWeightedAdjMatrix(double **weightedAdjMatrix,double **data_points,int num_of_cord,int num_of_point, int i)
{
    int j=0;
    weightedAdjMatrix[i] = caluc(num_of_point, sizeof(double));
    for (j = 0; j < num_of_point; j++)
    {
        if (j != i)
        {
            weightedAdjMatrix[i][j] = callDisBetweenToVector(data_points[i], data_points[j++],num_of_cord);
        }
        else
        {
            weightedAdjMatrix[i][j] = 0;
        }
    }
}
double callDisBetweenToVector(double *first_cord, double *second_cord,int num_of_cord)
{
    double distance = 0;
    int i;
    for(i=0;i<num_of_cord;i++ ) 
    {
        distance += pow((first_cord[i] - second_cord[i]), 2.0);
        first_cord = first_cord->next;
        second_cord = second_cord->next;
    }
    return exp2(-distance / 2);
}

double **createDegMatrix(double **weightedMatrix, int n)
{
    double **degMatrix = caluc(n, sizeof(double));
    int i, j;
    for (i = 0; i < n; i++)
    {
        degMatrix[i] = caluc(n, sizeof(double));
        for (j = 0; j < n; j++)
        {
            if (j == i)
            {
                degMatrix[i][j] = calDeg(weightedMatrix, i, n);
            }
            else
            {
                degMatrix[i][j] = 0;
            }
        }
    }
}

double calDeg(double **weightedMatrix, int i, int n)
{
    double sum = 0;
    int j;
    for (j = 0; j < n; j++)
    {
        sum += weightedMatrix[i][j];
    }
    return sum;
}

double **createGraphLaplacian(double **weightedMatrix, double **degMatrix, int n)
{
    double **laplacianMatrix = caluc(n, sizeof(double));
    int i, j;
    for (i = 0; i < n; i++)
    {
        laplacianMatrix[i] = caluc(n, sizeof(double));
        for (j = 0; j < n; j++)
        {
            laplacianMatrix[i][j] = degMatrix[i][j] - weightedMatrix[i][j];
        }
    }
    return laplacianMatrix;
}

/* TODO: move to spkmeans */
int calcKUsingEigengapHeuristic(double *eigenValues, int n)
{
    int maxIndex = 0;
    double maxDiff = 0;
    double currDif;
    int i;
    for (i = 0; i < n / 2; i++)
    {
        currDif = fabs(eigenValues[i] - eigenValues[i + 1]);
        if (currDif > maxDiff)
        {
            maxDiff = currDif;
            maxIndex = i;
        }
    }
    return maxIndex;
}

double **createUMatrix(double **eigenVectors, int k, int n)
{
    double **uMatrix = caluc(k, sizeof(double));
    int i, j;
    for (i = 0; i < k; i++)
    {
        uMatrix[i] = caluc(n, sizeof(double));
        for (j = 0; j < n; j++)
        {
            uMatrix[i][j] = eigenVectors[i][j];
        }
    }
    return uMatrix;
}