#include "file_utils.h"

FILE *readFile(char *fileName)
{
    FILE *fp;

    fp = fopen(fileName, "r");
    assert(fp != NULL);

    return fp;
}

int calcNumOfPoint(FILE *file)
{
    int num_of_points = 0;
    char c;

    for (c = getc(file); c != EOF; c = getc(file))
        if (c == '\n')
            num_of_points++;

    rewind(file);
    return num_of_points;
}

int calcNumOfCords(FILE *file)
{
    int num_of_cords = 0;
    char c;

    for (c = getc(file); c != '\n'; c = getc(file))
        if (c == ',')
            num_of_cords++;

    rewind(file);
    return ++num_of_cords;
}

double **fillDataPoints(FILE *file, int num_of_cords, int num_of_points)
{
    int i = 0, j;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *cord;
    double **data_points = (double **)malloc(num_of_points * sizeof(double *));
    assert(data_points != NULL);

    while ((read = getline(&line, &len, file)) != -1)
    {
        data_points[i] = (double *)malloc(num_of_cords * sizeof(double));
        assert(data_points[i] != NULL);

        cord = strtok(line, ",");
        for (j = 0; j < num_of_cords; j++)
        {
            if (j == num_of_cords - 1)
            {
                cord = strtok(cord, "\n");
            }

            data_points[i][j] = strtod(cord, &cord + sizeof(double));
            cord = strtok(NULL, ",");
        }

        i++;
    }

    free(line);
    rewind(file);
    return data_points;
}

double **readDataPointsFromFile(char *file_name) {
    int num_of_point, num_of_cords;
    FILE *f = readFile(file_name);
    double **data_points;
    assert(f != NULL);

    num_of_point = calcNumOfPoint(f);
    num_of_cords = calcNumOfCords(f);

    data_points = fillDataPoints(f, num_of_cords, num_of_point);
    fclose(f);

    return data_points; 
}

int sanity_test() {
    FILE *f = readFile("test_eigen_params.txt");
    int num_of_point, num_of_cords;
    int i;
    double **data_points;

    num_of_point = calcNumOfPoint(f);
    num_of_cords = calcNumOfCords(f);

    printf("%d\n", num_of_point);
    printf("%d\n", num_of_cords);

    data_points = fillDataPoints(f, num_of_cords, num_of_point);
    fclose(f);

    for (i = 0; i < num_of_point; i++) {
        free(data_points[i]);
    }
    free(data_points);

    return 0;
}