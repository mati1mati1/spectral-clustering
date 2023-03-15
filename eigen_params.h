#include "spkmeans.h"

#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#define DEBUG_EXEC(x) x
#else
#define DEBUG_PRINT(x) \
    do                 \
    {                  \
    } while (0)
#define DEBUG_EXEC(x) \
    do                \
    {                 \
    } while (0)
#endif

#define JACOBI_MAX_ROTATIONS 100
#define JACOBI_EPSILON 0.00001

typedef struct rotationParams
{
    int i;
    int j;
    double phi;
    double c;
    double s;
    double t;
} rotationParams;

typedef struct eigenParam
{
    double eigen_value;
    double *eigen_vector;
} eigenParam;

double **vectorToPointsArray(vector *v, int n);
double *cordToArray(cord *cord, int n);
/*
void freePointsArray(double **);
*/
void printPointsArray(double **mat, int rows, int columns);

void findMaxOffDiagPoint(double **mat, int mat_size, rotationParams *rotation_params);
double calcPhi(double **mat, rotationParams *rotation_params);
double calcT(double phi);
double calcC(double t);
double calcS(double t, double c);
int sign(double);
double calcA_i_i(double a_i_i, double a_j_j, double a_i_j, double c, double s);
double calcA_j_j(double a_i_i, double a_j_j, double a_i_j, double c, double s);
double calcA_r_i(double a_r_i, double a_r_j, double c, double s);
double calcA_r_j(double a_r_i, double a_r_j, double c, double s);

double **createIdentityMatrix(int mat_size);
double **matrixMul(double **first_mat, double **second_mat, int mat_size);
double rowColumnDotProduct(double **first_mat, double **second_mat, int row_index, int column_index, int mat_size);
double **createUpdatedPointsArray(double **mat, rotationParams *rotation_params, int mat_size);
double **createRotationMatrix(rotationParams *rotation_params, int mat_size);

double offDiagonalSquareSumDistance(double **first_mat, double **second_mat, int mat_size);
double offDiagonalSquareSum(double **mat, int mat_size);
int isConverged(double **points_array, double **updated_points_array, int iterations, int mat_size);

eigenParam **createEigenParams(double **points_array, double **eigen_vectors, int mat_size);
double *extractEigenVectorFromColumn(double **eigen_vectors, int column_index, int mat_size);
void printEigenParams(eigenParam **eigenParams, int count);
int eigen_param_cmp(const void *a, const void *b);

/* eigenParams *findEigenParams(double **GraphLaplacian, int n); */