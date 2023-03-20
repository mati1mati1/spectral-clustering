#include <assert.h>
#include "file_utils.h"
#include "matrix.h"
#include "jacobi.h"
#include "spkmeans.h"

void freePointsArray(double **points_array, int num_of_points)
{
    int i;

    for (i = 0; i < num_of_points; i++)
    {
        free(points_array[i]);
    }

    free(points_array);
}

int main(int argc, char *argv[])
{
    int num_of_points, num_of_cords;
    double **data_points;
    double **wam, **ddg, **gl;
    eigenParam **jacobi_results;
    char *goal = argv[1];
    char *file_name = argv[2];

    FILE *f = readFile(file_name);

    assert(argc > 0);

    num_of_points = calcNumOfPoint(f);
    num_of_cords = calcNumOfCords(f);

    data_points = fillDataPoints(f, num_of_cords, num_of_points);
    fclose(f);

    if (strcmp(goal, "wam") == 0)
    {
        wam = createWeightedAdjMatrix(data_points, num_of_points, num_of_cords);
        printPointsArray(wam, num_of_points, num_of_points);
        freePointsArray(wam, num_of_points);
    }
    else if (strcmp(goal, "ddg") == 0)
    {
        wam = createWeightedAdjMatrix(data_points, num_of_points, num_of_cords);
        ddg = createDegMatrix(wam, num_of_points);
        printPointsArray(ddg, num_of_points, num_of_points);
        freePointsArray(wam, num_of_points);
        freePointsArray(ddg, num_of_points);
    }
    else if (strcmp(goal, "gl") == 0)
    {
        wam = createWeightedAdjMatrix(data_points, num_of_points, num_of_cords);
        ddg = createDegMatrix(wam, num_of_points);
        gl = createGraphLaplacian(wam, ddg, num_of_points);
        printPointsArray(gl, num_of_points, num_of_points);
        freePointsArray(wam, num_of_points);
        freePointsArray(ddg, num_of_points);
        freePointsArray(gl, num_of_points);
    }
    else if (strcmp(goal, "jacobi") == 0)
    {
        jacobi_results = jacobi(data_points, num_of_points);
        printEigenParams(jacobi_results, num_of_points);
        freeEigenParams(jacobi_results, num_of_points);
    }
    else
    {
        printf("invalid goal");
    }
    
    freePointsArray(data_points, num_of_points);

    return 0;
}