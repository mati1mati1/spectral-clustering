#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "jacobi.h"
#include "file_utils.h"
#include "spkmeans.h"

double calcPhi(double **mat, rotationParams *rotation_params)
{
    return (mat[rotation_params->j][rotation_params->j] - mat[rotation_params->i][rotation_params->i]) / (2 * mat[rotation_params->i][rotation_params->j]);
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

double calcT(double phi)
{
    return sign(phi) / (fabs(phi) + sqrt(pow(phi, 2.0) + 1));
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

double calcA_i_i(double a_i_i, double a_j_j, double a_i_j, double c, double s)
{
    return pow(c, 2) * a_i_i + pow(s, 2) * a_j_j - 2 * s * c * a_i_j;
}

double calcA_j_j(double a_i_i, double a_j_j, double a_i_j, double c, double s)
{
    return pow(c, 2) * a_j_j + pow(s, 2) * a_i_i + 2 * s * c * a_i_j;
}

double calcA_r_i(double a_r_i, double a_r_j, double c, double s)
{
    return c * a_r_i - s * a_r_j;
}

double calcA_r_j(double a_r_i, double a_r_j, double c, double s)
{
    return c * a_r_j + s * a_r_i;
}

double **createUpdatedPointsArray(double **points_array, rotationParams *rotation_params, int mat_size)
{
    int i, j;
    double **update_points_array;

    update_points_array = (double **)malloc(mat_size * sizeof(double *));
    assert(update_points_array != NULL);

    for (i = 0; i < mat_size; i++)
    {
        update_points_array[i] = malloc(mat_size * sizeof(double));
        assert(update_points_array[i] != NULL);

        for (j = 0; j < mat_size; j++)
        {
            /* A'(i,i) */
            if (i == rotation_params->i && j == rotation_params->i)
            {
                update_points_array[i][j] = calcA_i_i(
                    points_array[rotation_params->i][rotation_params->i],
                    points_array[rotation_params->j][rotation_params->j],
                    points_array[rotation_params->i][rotation_params->j],
                    rotation_params->c,
                    rotation_params->s);
                continue;
            }

            /* A'(j,j) */
            if (i == rotation_params->j && j == rotation_params->j)
            {
                update_points_array[i][j] = calcA_j_j(
                    points_array[rotation_params->i][rotation_params->i],
                    points_array[rotation_params->j][rotation_params->j],
                    points_array[rotation_params->i][rotation_params->j],
                    rotation_params->c,
                    rotation_params->s);
                continue;
            }

            /* A'(i,j) or A'(j,i) */
            if ((i == rotation_params->j && j == rotation_params->i) || (i == rotation_params->i && j == rotation_params->j))
            {
                update_points_array[i][j] = 0;
                continue;
            }

            /* A'(r,i) where r != i, j */
            if (j == rotation_params->i)
            {
                update_points_array[i][j] = calcA_r_i(
                    points_array[i][rotation_params->i],
                    points_array[i][rotation_params->j],
                    rotation_params->c,
                    rotation_params->s);
                continue;
            }

            /* A'(i,r) where r != i, j */
            if (i == rotation_params->i)
            {
                update_points_array[i][j] = calcA_r_i(
                    points_array[rotation_params->i][j],
                    points_array[rotation_params->j][j],
                    rotation_params->c,
                    rotation_params->s);
                continue;
            }

            /* A'(r,j) where r != i, j */
            if (j == rotation_params->j)
            {
                update_points_array[i][j] = calcA_r_j(
                    points_array[i][rotation_params->i],
                    points_array[i][rotation_params->j],
                    rotation_params->c,
                    rotation_params->s);
                continue;
            }

            /* A'(j,r) where r != i, j */
            if (i == rotation_params->j)
            {
                update_points_array[i][j] = calcA_r_j(
                    points_array[rotation_params->i][j],
                    points_array[rotation_params->j][j],
                    rotation_params->c,
                    rotation_params->s);
                continue;
            }

            /* otherwise */
            update_points_array[i][j] = points_array[i][j];
        }
    }

    return update_points_array;
}

double **createRotationMatrix(rotationParams *rotation_params, int mat_size)
{
    int i, j;
    double **rotation_matrix = malloc(mat_size * sizeof(double *));
    assert(rotation_matrix != NULL);

    for (i = 0; i < mat_size; i++)
    {
        rotation_matrix[i] = malloc(mat_size * sizeof(double));
        for (j = 0; j < mat_size; j++)
        {
            if (i == j && (i == rotation_params->i || j == rotation_params->j))
            {
                rotation_matrix[i][j] = rotation_params->c;
                continue;
            }

            if (i == j)
            {
                rotation_matrix[i][j] = 1;
                continue;
            }

            if ((i == rotation_params->i && j == rotation_params->j))
            {
                rotation_matrix[i][j] = rotation_params->s;
                continue;
            }

            if ((j == rotation_params->i && i == rotation_params->j))
            {
                rotation_matrix[i][j] = -1 * rotation_params->s;
                continue;
            }

            rotation_matrix[i][j] = 0;
        }
    }

    return rotation_matrix;
}

double offDiagonalSquareSumDistance(double **first_mat, double **second_mat, int mat_size)
{
    return offDiagonalSquareSum(first_mat, mat_size) - offDiagonalSquareSum(second_mat, mat_size);
}

double offDiagonalSquareSum(double **mat, int mat_size)
{
    double squared_sum = 0;
    int i, j;

    for (i = 0; i < mat_size; i++)
    {
        for (j = 0; j < mat_size; j++)
        {
            if (i == j)
                continue;

            squared_sum += pow(mat[i][j], 2.0);
        }
    }

    return squared_sum;
}

int isConverged(double **points_array, double **updated_points_array, int iterations, int mat_size)
{
    return iterations >= JACOBI_MAX_ROTATIONS ||
           offDiagonalSquareSumDistance(points_array, updated_points_array, mat_size) <= JACOBI_EPSILON;
}

void printPointsArray(double **mat, int rows, int columns)
{
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < columns - 1; j++)
        {
            printf("%.4f,", mat[i][j]);
        }
        printf("%.4f\n", mat[i][j]);
    }
}

double **createIdentityMatrix(int mat_size)
{
    double **identity_matrix;
    int i;

    identity_matrix = malloc(mat_size * sizeof(double *));
    assert(identity_matrix);

    for (i = 0; i < mat_size; i++)
    {
        identity_matrix[i] = calloc(mat_size, sizeof(double));
        identity_matrix[i][i] = 1;
    }

    return identity_matrix;
}

double **matrixMul(double **first_mat, double **second_mat, int mat_size)
{
    int i, j;
    double **result = malloc(mat_size * sizeof(double *));

    for (i = 0; i < mat_size; i++)
    {
        result[i] = malloc(mat_size * sizeof(double));

        for (j = 0; j < mat_size; j++)
        {
            result[i][j] = rowColumnDotProduct(first_mat, second_mat, i, j, mat_size);
        }
    }

    return result;
}

double rowColumnDotProduct(double **first_mat, double **second_mat, int row_index, int column_index, int mat_size)
{
    double result = 0;
    int i;

    for (i = 0; i < mat_size; i++)
    {
        result += first_mat[row_index][i] * second_mat[i][column_index];
    }

    return result;
}

eigenParam **createEigenParams(double **points_array, double **eigen_vectors, int mat_size)
{
    int i;
    eigenParam **eigen_params = malloc(mat_size * sizeof(eigenParam *));
    assert(eigen_params != NULL);

    for (i = 0; i < mat_size; i++)
    {
        eigenParam *eigen_param = malloc(sizeof(eigenParam));
        assert(eigen_param != NULL);

        eigen_param->eigen_value = points_array[i][i];
        eigen_param->eigen_vector = extractEigenVectorFromColumn(eigen_vectors, i, mat_size);

        eigen_params[i] = eigen_param;
    }

    return eigen_params;
}

double *extractEigenVectorFromColumn(double **eigen_vectors, int column_index, int mat_size)
{
    int i;
    double *eigen_vector = malloc(mat_size * sizeof(double));
    assert(eigen_vector != NULL);

    for (i = 0; i < mat_size; i++)
    {
        eigen_vector[i] = eigen_vectors[i][column_index];
    }

    return eigen_vector;
}

void debugPrintEigenParams(eigenParam **eigen_params, int count)
{
    int i, j;

    printf("\n****** Eigen Params ******\n");
    for (i = 0; i < count; i++)
    {
        printf("\n%d:\n", i);
        printf("Eigen Value: %.4f\n", eigen_params[i]->eigen_value);
        printf("Eigen Vector: [");

        for (j = 0; j < count - 1; j++)
        {
            printf("%.4f, ", eigen_params[i]->eigen_vector[j]);
        }

        printf("%.4f]\n", eigen_params[i]->eigen_vector[j]);
    }
}

int eigen_param_cmp(const void *a, const void *b)
{
    return ((*(eigenParam **)a)->eigen_value - (*(eigenParam **)b)->eigen_value);
}

eigenParam **jacobi(double **data_points, int n)
{
    double **updated_points_array, **updated_eigen_vectors, **rotation_matrix;
    int converged, iterations = 0;
    eigenParam **eigen_params;
    rotationParams *rotation_params = malloc(sizeof(rotationParams));
    double **eigen_vectors = createIdentityMatrix(n);

    do
    {
        iterations++;
        findMaxOffDiagPoint(data_points, n, rotation_params);

        rotation_params->phi = calcPhi(data_points, rotation_params);
        rotation_params->t = calcT(rotation_params->phi);
        rotation_params->c = calcC(rotation_params->t);
        rotation_params->s = calcS(rotation_params->t, rotation_params->c);

        /* calculates the updated A' */
        updated_points_array = createUpdatedPointsArray(data_points, rotation_params, n);

        /* calculates the rotation matrix P */
        rotation_matrix = createRotationMatrix(rotation_params, n);

        /* updated the V matrix using the rotation matrix */
        updated_eigen_vectors = matrixMul(eigen_vectors, rotation_matrix, n);

        freePointsArray(eigen_vectors, n);
        eigen_vectors = updated_eigen_vectors;

        converged = isConverged(data_points, updated_points_array, iterations, n);

        /* we don't want to free the given initial data_points */
        if (iterations > 1)
            freePointsArray(data_points, n);

        freePointsArray(rotation_matrix, n);

        data_points = updated_points_array;

    } while (!converged);

    eigen_params = createEigenParams(data_points, eigen_vectors, n);
    qsort(eigen_params, n, sizeof(eigenParam *), eigen_param_cmp);

    free(rotation_params);
    freePointsArray(eigen_vectors, n);
    freePointsArray(data_points, n);

    return eigen_params;
}

void printEigenParams(eigenParam **eigen_params, int n)
{
    printEigenValues(eigen_params, n);
    printEigenVectors(eigen_params, n);
}

void printEigenValues(eigenParam **eigen_params, int n)
{
    int i;

    for (i = 0; i < n - 1; i++)
    {
        printf("%.4f,", eigen_params[i]->eigen_value);
    }

    printf("%.4f\n", eigen_params[i]->eigen_value);
}

void printEigenVectors(eigenParam **eigen_params, int n)
{
    int i, j;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n - 1; j++)
        {
            printf("%.4f,", eigen_params[j]->eigen_vector[i]);
        }

        printf("%.4f\n", eigen_params[j]->eigen_vector[i]);
    }
}

void freeEigenParams(eigenParam **eigen_params, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        free(eigen_params[i]->eigen_vector);
        free(eigen_params[i]);
    }

    free(eigen_params);
}

int test_main()
{
    double **points_array, **updated_points_array, **eigen_vectors, **updated_eigen_vectors, **rotation_matrix;
    int n = 3, converged, iterations = 0;
    eigenParam **eigen_params;
    rotationParams *rotation_params = malloc(sizeof(rotationParams));
    points_array = readDataPointsFromFile("test_eigen_params.txt");
    eigen_vectors = createIdentityMatrix(n);

    do
    {
        iterations++;
        DEBUG_PRINT(("********************************\n"));
        DEBUG_PRINT(("iteration: [%d]\n", iterations));
        DEBUG_PRINT(("A:\n"));
        DEBUG_EXEC((printPointsArray(points_array, n, n)));

        findMaxOffDiagPoint(points_array, n, rotation_params);
        DEBUG_PRINT(("i: [%d], j: [%d], A_i_j: [%f] \n", rotation_params->i, rotation_params->j, points_array[rotation_params->i][rotation_params->j]));

        rotation_params->phi = calcPhi(points_array, rotation_params);
        DEBUG_PRINT(("phi: [%f]\n", rotation_params->phi));

        rotation_params->t = calcT(rotation_params->phi);
        rotation_params->c = calcC(rotation_params->t);
        rotation_params->s = calcS(rotation_params->t, rotation_params->c);

        DEBUG_PRINT(("t: [%f], c: [%f], s: [%f]\n", rotation_params->t, rotation_params->c, rotation_params->s));

        /* calculates the updated A' */
        updated_points_array = createUpdatedPointsArray(points_array, rotation_params, n);
        DEBUG_PRINT(("A':\n"));
        DEBUG_EXEC((printPointsArray(updated_points_array, n, n)));

        /* calculates the rotation matrix P */
        rotation_matrix = createRotationMatrix(rotation_params, n);
        DEBUG_PRINT(("P:\n"));
        DEBUG_EXEC((printPointsArray(rotation_matrix, n, n)));

        /* updated the V matrix using the rotation matrix */
        updated_eigen_vectors = matrixMul(eigen_vectors, rotation_matrix, n);
        /*
        freePointsArray(eigen_vectors);
        */
        eigen_vectors = updated_eigen_vectors;

        DEBUG_PRINT(("V:\n"));
        DEBUG_EXEC((printPointsArray(eigen_vectors, n, n)));

        converged = isConverged(points_array, updated_points_array, iterations, n);

        /*
        freePointsArray(points_array);
        freePointsArray(rotation_matrix);
        */

        points_array = updated_points_array;

    } while (!converged);

    eigen_params = createEigenParams(points_array, eigen_vectors, n);
    DEBUG_EXEC((debugPrintEigenParams(eigen_params, n)));
    qsort(eigen_params, n, sizeof(eigenParam *), eigen_param_cmp);
    DEBUG_EXEC((debugPrintEigenParams(eigen_params, n)));

    return 0;
}
