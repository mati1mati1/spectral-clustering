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

