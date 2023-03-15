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
double **createWeightedAdjMatrix(vector* initalDataPoints, int n); // Todo: Matan
double **createDegMatrix(double** weightedMatrix, int n); // Todo: Matan
double **createGraphLaplacian(double** weightedMatrix, double** degMatrix, int n); // Todo: Matan
eigenParams *findEigenParams(double** GraphLaplacian, int n); // Todo: Shahaf
int calcKUsingEigengapHeuristic(double* eigenValues, int n); // Todo: Matan

void fillRowInWeightedAdjMatrix(double ** weightedAdjMatrix,vector* head,vector* curent,int n, int i);
double callDisBetweenToVector(cord* first_cord,cord* second_cord);
double calDeg(double** weightedMatrix, int i, int n);
/* The matrix U is the given matrix for the k-means++ algo */
vector *createUMatrix(cord **eigenVectors, int k);

