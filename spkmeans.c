#include "file_utils.h"

int main(int argc, char *argv[]) {
    char *goal = argv[1];
    char *file_name = argv[2];
    double **data_points = readDataPointsFromFile(file_name);
    
}