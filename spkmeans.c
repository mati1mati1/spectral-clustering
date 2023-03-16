#include <assert.h>
#include "file_utils.h"
#include "matrix.h"
#include "eigen_params.h"


int main(int argc, char *argv[])
{
    int num_of_points, num_of_cords;
    double **data_points;
    double **wam; /*, **ddg, **gl;*/
    eigenParam **jacobi_results;
    char *goal = argv[1];
    char *file_name = argv[2];
    assert(argc > 0);

    FILE *f = readFile(file_name);

    num_of_points = calcNumOfPoint(f);
    num_of_cords = calcNumOfCords(f);

    data_points = fillDataPoints(f, num_of_cords, num_of_points);
    fclose(f);

    if (strcmp(goal, "wam") == 0)
    {
        wam = createWeightedAdjMatrix(data_points, num_of_points, num_of_cords);
        printPointsArray(wam, num_of_points, num_of_cords);
    }
    else if (strcmp(goal, "ddg") == 0)
    {
    }
    else if (strcmp(goal, "gl") == 0)
    {
    }
    else if (strcmp(goal, "jacobi") == 0)
    {
        jacobi_results = jacobi(data_points, num_of_points);
        printf("%f", jacobi_results[0]->eigen_value);
    }
    else
    {
        printf("invalid goal");
    }

    return 0;
}