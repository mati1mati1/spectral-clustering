#include <assert.h>
#include "file_utils.h"
/* #include "matrix.h" */
#include "eigen_params.h"

int main(int argc, char *argv[]) {
    char *goal = argv[1];
    char *file_name = argv[2];
    double **data_points = readDataPointsFromFile(file_name);
    /*double **wam, **ddg, **gl;*/
    eigenParam **jacobi_results;

    assert(argc > 0);

    if (strcmp(goal, "wam") == 0) {

    } else if (strcmp(goal, "ddg") == 0) {

    } else if (strcmp(goal, "gl") == 0) {

    } else if (strcmp(goal, "jacobi") == 0) {
        jacobi_results = jacobi(data_points, 3);
        printf("%f", jacobi_results[0]->eigen_value);        
    } else {
        printf("invalid goal");
    }

    return 0;
}