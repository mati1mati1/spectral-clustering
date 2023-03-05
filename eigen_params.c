#include "spkmeans.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x) \
    do                 \
    {                  \
    } while (0)
#endif

vector *fillDataPoint();
void printVector(vector *);
int countPointsInVector(vector *pointsVector);

typedef struct rotationParams
{
    int i;
    int j;
    double phi;
    double c;
    double s;
    double t;
} rotationParams;

double **vectorToPointsArray(vector *v, int n);
double *cordToArray(cord *cord, int n);
void freePointsArray(double **);

void findMaxOffDiagPoint(double **mat, int mat_size, rotationParams *rotation_params);
double calcPhi(double **mat, int mat_size, rotationParams *rotation_params);
double calcT(double phi);
double calcC(double t);
double calcS(double t, double c);
int sign(double);

double **updatePointsArray(double **mat, rotationParams *rotation_params);
double **createRotationMatrix(double **mat, rotationParams *rotation_params);
void updateEigenVectorsMatrix(double **prev_matrix, double **rotation_matrix);
int isConverged(double **points_array, double **updated_points_array, int iterations);

double calcPhi(double **mat, int mat_size, rotationParams *rotation_params)
{
    return (mat[rotation_params->j][rotation_params->j] - mat[rotation_params->i][rotation_params->i]) / 2 * mat[rotation_params->i][rotation_params->j];
}

/* finds the off diagonal element with the largest absolute value */
void findMaxOffDiagPoint(double **mat, int mat_size, rotationParams *rotation_params)
{
    int i, j;
    rotation_params->i = 0;
    rotation_params->j = 1;

    for (i = 0; i < mat_size; i++)
    {
        for (j = 0; j < mat_size; j++)
        {
            /* we looking for the off diagonal largest number */
            if (i == j)
            {
                continue;
            }

            if (fabs(mat[i][j]) > fabs(mat[rotation_params->i][rotation_params->j]))
            {
                rotation_params->i = i;
                rotation_params->j = j;
            }
        }
    }
}

double **vectorToPointsArray(vector *v, int n)
{
    double **points_array;
    int i;

    points_array = malloc(n * sizeof(double *));
    assert(points_array);

    for (i = 0; i < n; i++)
    {
        points_array[i] = cordToArray(v->cords, n);
        v = v->next;
    }

    return points_array;
}

double *cordToArray(cord *cord, int n)
{
    double *cord_arr;
    int i;

    cord_arr = malloc(n * sizeof(double));
    assert(cord_arr);

    for (i = 0; i < n; i++)
    {
        cord_arr[i] = cord->value;
        cord = cord->next;
    }

    return cord_arr;
}

void freePointsArray(double **mat)
{
}

double calcT(double phi)
{
    /* since pow(0, exp) yields 0 instead of 1 */
    int phi_squared = (fabs(phi) == 0) ? 1 : pow(phi, 2.0);
    return sign(phi) / (fabs(phi) + sqrt(phi_squared + 1));
}

int sign(double x)
{
    if (x > 0)
        return 1;
    if (x < 0)
        return -1;
    /* we define sign(0) = 1*/
    return 1;
}

double calcC(double t)
{
    return 1 / sqrt(pow(t, 2.0) + 1);
}

double calcS(double t, double c)
{
    return t * c;
}

double **updatePointsArray(double **mat, rotationParams *rotationParams)
{
}

double **createRotationMatrix(double **mat, rotationParams *rotationParams)
{
    return NULL;
}

void updateEigenVectorsMatrix(double **prev_matrix, double **rotation_matrix)
{
}

int isConverged(double **points_array, double **updated_points_array, int iterations)
{
    return 0;
}

int main()
{
    vector *points_vector;
    double **points_array, **updated_points_array, **eigen_vectors, **rotation_matrix;
    int n, converged, iterations = 0;
    double phi, t, c, s;
    rotationParams *rotation_params = malloc(sizeof(rotation_params));
    ;

    points_vector = fillDataPoint();
    n = countPointsInVector(points_vector);

    DEBUG_PRINT(("n: [%d]\n", n));

    points_array = vectorToPointsArray(points_vector, n);

    do
    {
        iterations++;
        DEBUG_PRINT(("********************************\n"));
        DEBUG_PRINT(("iteration: [%d]\n", iterations));

        findMaxOffDiagPoint(points_array, n, rotation_params);
        DEBUG_PRINT(("i: [%d], j: [%d], A_i_j: [%f] \n", rotation_params->i, rotation_params->j, fabs(points_array[rotation_params->i][rotation_params->j])));

        rotation_params->phi = calcPhi(points_array, n, rotation_params);
        DEBUG_PRINT(("phi: [%f]\n", rotation_params->phi));

        rotation_params->t = calcT(rotation_params->phi);
        rotation_params->c = calcC(rotation_params->t);
        rotation_params->s = calcS(rotation_params->t, rotation_params->c);

        DEBUG_PRINT(("t: [%f], c: [%f], s: [%f]\n", rotation_params->t, rotation_params->c, rotation_params->s));

        /* calculates the updated A' */
        updated_points_array = updatePointsArray(points_array, rotation_params);

        /* calculates the rotation matrix P */
        rotation_matrix = createRotationMatrix(points_array, rotation_params);

        /* updated the V matrix using the rotation matrix */
        updateEigenVectorsMatrix(eigen_vectors, rotation_matrix);

        converged = isConverged(points_array, updated_points_array, iterations);

        freePointsArray(points_array);
        freePointsArray(rotation_matrix);

        points_array = updated_points_array;

    } while (!converged);

    /* create the eigen_params struct:

    1. extractEigenValuesFromDiagonalMatrix(points_array); -> new struct that contains the values and original indexes;
    2. qSort by the new struct (by values);
    3. run over the sorted list and create a new sorted_eigen_vectors matrix (free the old one afterwards);
    4. malloc the eigen_params struct:
        eigen_params->eigenValues = eigenValues(extract_eigen_values);
        eigen_params->eigenVectors = sorted_eigen_vectors;

    */

    /* calc the off(A)^2 - off(A')^2 */
    /* converges when epsilon = 1.0 * 10 ^ -5 or 100 rotations */

    return 0;
}
