typedef struct cord
{
    double value;
    struct cord *next;
} cord ;

typedef struct vector
{
    cord *cords;
    struct vector *next;
} vector ;

typedef struct eigenParams {
    cord *eigenValues;
    cord **eigenVectors;
} eigenParams ;

// given the initial data set contains n points
cord **createWeightedAdjMatrix(vector* initalDataPoints, int n); // Todo: Matan
cord **createDegMatrix(cord** weightedMatrix, int n); // Todo: Matan
cord **createGraphLaplacian(cord** weightedMatrix, cord** degMatrix, int n); // Todo: Matan
eigenParams *findEigenParams(cord** GraphLaplacian, int n); // Todo: Shahaf
int calcKUsingEigengapHeuristic(cord *eigenValues, int n); // Todo: Matan

// The matrix U is the given matrix for the k-means++ algo
vector *createUMatrix(cord **eigenVectors, int k); // Todo: Matan




