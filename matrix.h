/* given the initial data set contains n points */
double **createWeightedAdjMatrix(double **data_points, int num_of_point, int num_of_cord);
double **createDegMatrix(double **weightedMatrix, int n);
double **createGraphLaplacian(double **weightedMatrix, double **degMatrix, int n);
double callDisBetweenToVector(double *first_cord, double *second_cord, int num_of_cords);
double calDeg(double **weightedMatrix, int i, int n);
double **createGraphLaplacian(double **weightedMatrix, double **degMatrix, int n);
void fillRowInWeightedAdjMatrix(double **weightedAdjMatrix, double **data_points, int num_of_cord, int num_of_point, int i);