typedef struct cord
{
    double value;
    struct cord *next;
} cord;

typedef struct vector
{
    cord *cords;
    struct vector *next;
} vector;

typedef struct eigenParams
{
    double *eigenValues;
    double **eigenVectors;
} eigenParams;

/* given the initial data set contains n points */
cord **createWeightedAdjMatrix(vector *initalDataPoints, int n);
cord **createDegMatrix(cord **weightedMatrix, int n);
cord **createGraphLaplacian(cord **weightedMatrix, cord **degMatrix, int n);
eigenParams *findEigenParams(cord **GraphLaplacian, int n);
int calcKUsingEigengapHeuristic(cord *eigenValues, int n);

/* The matrix U is the given matrix for the k-means++ algo */
vector *createUMatrix(cord **eigenVectors, int k);
