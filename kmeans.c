#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define  _POSIX_C_SOURCE 200809L
#define MAX_ITER 1000
#define EPSILON 0.001

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
int check_epsilon_value(cord **clusters, cord **updated_clusters, int k);

int validateIter(char *iter);
int isNaturalNumber(char *c);
int all_zeroes(char *c);
int validateK(char *k, vector *pointsVector);

void free_vector(vector *v);
void free_cord(cord *c);
void free_cords_array(cord **arr, int k);

vector *fillDataPoint(){
    vector *head_vec, *curr_vec;
    cord *head_cord, *curr_cord;
    double n;
    char c;

    head_cord = malloc(sizeof(cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;
    while (scanf("%lf%c", &n, &c) == 2)
    {

        if (c == '\n')
        {
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(vector));
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(cord));
            curr_cord = head_cord;
            curr_cord->next = NULL;
            continue;
        }

        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(cord));
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
    }

    free_cord(curr_cord);
    return head_vec;
}

void printVector(vector *v) {
    while (v != NULL) {
        printCord(v->cords);
        v = v->next;
    }
    printf("\n");
}

void printCord(cord *c) {
    do {
        printf("%.4f,", c->value);
        c = c->next;
    }
    while (c->next != NULL);
    printf("%.4f\n", c->value);
}

int validateIter(char *iter) {
    int intIter;
    
    if (isNaturalNumber(iter) != 0) {
        printf("Invalid maximum iteration!\n");
        return 1;    
    }

    intIter = atoi(iter);
    if (intIter >= MAX_ITER || intIter <= 1) {        
        printf("Invalid maximum iteration!\n");
        return 2;
    }

    return 0;
}

int validateK(char *k, vector *pointsVector) {
    int points;
    int intK;
    
    if (isNaturalNumber(k) != 0) {
        printf("Invalid number of clusters!\n");
        return 1;
    }

    points = countPointsInVector(pointsVector);
    intK = atoi(k);
    if (intK >= points || intK <= 1) {
        printf("Invalid number of clusters!\n");
        return 2;
    }

    return 0;
}

int countPointsInVector(vector *v) {
    int counter = 0;
    while (v->next != NULL) {
        counter++;
        v = v->next;
    }

    return counter;
}

/* returns 0 if it is a natural number, and 1 otherwise */ 
int isNaturalNumber(char *c) {
    /* in case the first char is . */
    if (*c == '.') {
        return 1;
    }
    
    while (*c != '\0') {
        if (*c == '.') {
            return all_zeroes(++c); 
        }

        if (isdigit(*c) == 0) {
            return 1;    
        }

        c++;
    }

    return 0;
}

int all_zeroes(char *c) {
    while (*c != '\0') {
        if (*c != '0') {
            return 1;
        }

        c++;
    }

    return 0; 
}


cord **initializeKCenter(int k, vector *points_vector) {
    vector *points_vector_vector;
    cord **clusters;
    cord *head_cord, *curr_cord, *points_vector_cord;
    int i = 0;

    head_cord = malloc(sizeof(cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    clusters = malloc(k * sizeof(cord*));

    points_vector_vector = points_vector;
    points_vector_cord = points_vector_vector->cords;

    while (k > 0)
    {
        while (points_vector_cord->next != NULL)
        {
            curr_cord->value = points_vector_cord->value;
            curr_cord->next = malloc(sizeof(cord));
            curr_cord = curr_cord->next;
            curr_cord->next = NULL;
            points_vector_cord = points_vector_cord->next;
        }

        clusters[i++] = head_cord;
        curr_cord->value = points_vector_cord->value;

        points_vector_vector = points_vector_vector->next;
        points_vector_cord = points_vector_vector->cords;

        head_cord = malloc(sizeof(cord));
        curr_cord = head_cord;
        curr_cord->next = NULL;
        
        k--;
    }

    free_cord(head_cord);
    return clusters;
}

int num_of_cords_in_cord(cord *c) {
    int counter = 0;
    
    while (c != NULL) {
        counter++;
        c = c->next;
    }

    return counter;
}

void normalize_updated_cluster(cord **updated_clusters, int *num_of_cords_in_cluster, int k) {
    cord *curr_cord;
    int i;

    for(i=0;i<k;i++){
        curr_cord = updated_clusters[i];

        while (curr_cord !=  NULL)
        {
            curr_cord->value /= num_of_cords_in_cluster[i];
            curr_cord = curr_cord->next; 
        }   
    }
}

void add_point_to_cluster(cord *points_vector_cords, cord *cluster_cord, int l) {
    int i;

    for(i=0; i<l; i++){
        cluster_cord->value = (cluster_cord->value) + (points_vector_cords->value);
        cluster_cord = cluster_cord->next;
        points_vector_cords = points_vector_cords->next;
    }
}

double calc_distance(cord *first_cord, cord *second_cord) {
    double distance = 0;
    
    while(first_cord != NULL){
        distance += pow((second_cord->value - first_cord->value), 2.0);
        first_cord = first_cord->next;
        second_cord = second_cord->next;
    }

    return sqrt(distance);
}

cord **create_updated_cluster(cord **clusters, int k, vector *points_vector) {
    cord **updated_clusters;
    cord *head_cord, *curr_cord;
    int *num_of_cords_in_cluster;
    int i, j, min_index, l = num_of_cords_in_cord(points_vector->cords);
    double min_distance, current_distance;

    updated_clusters = malloc(k * sizeof(cord*));
    num_of_cords_in_cluster = malloc(sizeof(int) * k);

    for (i = 0; i < k; i++) {
        head_cord = malloc(sizeof(cord));
        curr_cord = head_cord;
        curr_cord->next = NULL;
        
        for (j = 0; j < l - 1; j++) {
            curr_cord->value = 0;
            curr_cord->next = malloc(sizeof(cord));
            curr_cord = curr_cord->next;
            curr_cord->next = NULL;
        }
        curr_cord->value = 0;

        updated_clusters[i] = head_cord;
        num_of_cords_in_cluster[i] = 0;
    }

    while (points_vector->next != NULL) {
        min_distance = __DBL_MAX__;

        for (i = 0; i < k; i++) {
            current_distance = calc_distance(clusters[i], points_vector->cords);
            if (current_distance < min_distance) {
                min_index = i;
                min_distance = current_distance;
            }
        }

        add_point_to_cluster(points_vector->cords, updated_clusters[min_index], l);
        num_of_cords_in_cluster[min_index]++;
        points_vector = points_vector->next;
    } 

    normalize_updated_cluster(updated_clusters, num_of_cords_in_cluster, k);
    free(num_of_cords_in_cluster);
    return updated_clusters;
}

/*returns 0 if we should do another iteration according to the epsilon value*/
int check_epsilon_value(cord **clusters, cord **updated_clusters, int k) {
    int i;
    double distance;

    for (i = 0; i < k; i++) {
        distance = calc_distance(clusters[i], updated_clusters[i]);
        if (distance >= EPSILON) {
            return 0;
        }
    }

    return 1;
}

void print_cords_array(cord **cords, int len) {
    int i;

    for (i = 0; i < len; i++) {
        printCord(cords[i]);
    }
}

void free_vector(vector *v) {
    vector *tmp;

    while (v->next != NULL) {
        tmp = v;
        free_cord(tmp->cords);
        v = v->next;
        free(tmp);
    }

    free(v);
}

void free_cords_array(cord **arr, int len) {
    while (--len >= 0) {
        free_cord(arr[len]);
    }

    free(arr);
}

void free_cord(cord *c) {
    cord *tmp;

    while (c != NULL) {
        tmp = c;
        c = c->next;
        free(tmp);
    }
}

int main(int argc, char *argv[]){
    vector *pointsVector;
    cord **clusters;
    cord **updated_clusters;
    int maxOfIter;
    int valid;
    int k;

    if (argc > 2){
        valid = validateIter(argv[2]);
        if (valid != 0) {
            return 1;
        }

        maxOfIter = atoi(argv[2]);
    }
    else{
        maxOfIter = 200;
    }
    
    pointsVector = fillDataPoint();
    valid = validateK(argv[1], pointsVector);
    if (valid != 0) {
        free_vector(pointsVector);
        return 1;
    }

    k = atoi(argv[1]);

    clusters = initializeKCenter(k, pointsVector);
    
    while (maxOfIter > 0) {
        updated_clusters = create_updated_cluster(clusters, k, pointsVector);
        if (check_epsilon_value(clusters, updated_clusters, k)) {
            free_cords_array(clusters, k);
            clusters = updated_clusters;
            break;
        }

        free_cords_array(clusters, k);
        clusters = updated_clusters;
        maxOfIter -= 1;
    }

    print_cords_array(clusters, k);

    free_vector(pointsVector);
    free_cords_array(clusters, k);

    return 0;
}