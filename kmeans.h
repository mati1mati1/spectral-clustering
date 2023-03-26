#include <Python.h>

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

int countPointsInVector(vector *pointsVector);
void printVector(vector*);
void printCord(cord*);
double calc_distance(cord *first_cord, cord *second_cord);

vector *fillDataPoint();
cord **initializeKCenter(int k, vector *points_vector);
void normalize_updated_cluster(cord **updated_clusters, int *num_of_cords_in_cluster,int k);
void add_point_to_cluster(cord *points_vector_cords, cord *cluster_cord, int l);
int check_epsilon_value(cord **clusters, cord **updated_clusters, int k, double epsilon);

int validateIter(char *iter);
int isNaturalNumber(char *c);
int all_zeroes(char *c);
int validateK(char *k, vector *pointsVector);

void free_vector(vector *v);
void free_cord(cord *c);
void free_cords_array(cord **arr, int k);

PyObject *kmeans(int k, int maxOfIter, double epsilon, vector *pointsVector);
PyObject *parseClusters(cord **clusters, int k);
PyObject *fit(PyObject *self, PyObject *args);
