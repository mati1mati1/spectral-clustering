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


vector *fillDataPoint();
void printVector(vector *);
int countPointsInVector(vector *pointsVector);

void fillRowInWeightedAdjMatrix(double ** weightedAdjMatrix,vector* head,vector* curent,int n, int i);
double callDisBetweenToVector(cord* first_cord,cord* second_cord);
double calDeg(double** weightedMatrix, int i, int n);
/* The matrix U is the given matrix for the k-means++ algo */
double *createUMatrix(double **eigenVectors, int k);

int calcKUsingEigengapHeuristic(double *eigenValues, int n);