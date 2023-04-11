from typing import List

import spkmeansmodule
import sys
import pandas as pd
import numpy as np

np.random.seed(0)

GOAL_TO_C_MODULE_FUNCTION = {
    "ddg": spkmeansmodule.ddg,
    "wam": spkmeansmodule.wam,
    "gl": spkmeansmodule.gl,
    "jacobi": spkmeansmodule.jacobi,
}

ITERATIONS = 300
EPSILON = 0


def print_matrix(matrix_array):
    for centroid in matrix_array:
        print(",".join(("%.4f" % cord) for cord in centroid))


def print_indexes(indexes):
    print(",".join(str(index) for index in indexes))


def calc_k_using_eigengap_heuristic(eigen_values):
    gap_array = np.zeros(int(len(eigen_values) / 2))
    for i in range(int(len(eigen_values) / 2)):
        gap_array[i] = abs(eigen_values[i] - eigen_values[i + 1])

    return np.argmax(gap_array) + 1


def kmeans_pp(k: int, iterations: int, eps: int, data_points: List[List[float]]):
    data_points_df = pd.DataFrame(data_points)
    index = np.random.choice(data_points_df.index, 1)

    centroid_array = data_points_df.loc[index].values.copy()
    data_points_df = data_points_df.drop(index)

    indexes = [int(index)]
    while len(centroid_array) < k:
        data_points_df["distance"] = data_points_df.apply(lambda row: min(
            np.linalg.norm(i - row) for i in centroid_array), axis=1)
        data_points_df["ratio"] = data_points_df["distance"] / \
            sum(data_points_df["distance"])

        index = np.random.choice(
            data_points_df.index, 1, p=data_points_df["ratio"])
        indexes.append(int(index))

        row_to_append = data_points_df.loc[:, ~data_points_df.columns.isin(
            ["distance", "ratio"])].loc[index].values.copy()

        centroid_array = np.append(centroid_array, row_to_append, axis=0)

        data_points_df = data_points_df.drop(index)
        data_points_df = data_points_df.drop(["distance", "ratio"], axis=1)

    arr = centroid_array.tolist() + data_points_df.values.copy().tolist()

    print_indexes(indexes)
    return spkmeansmodule.spk(k, iterations, eps, arr)


def spkmeans(data_points: List[List[float]], k: int):
    gl_matrix = spkmeansmodule.gl(data_points)
    eigen_params = np.array(spkmeansmodule.jacobi(gl_matrix))

    # sorts the eigen params columns according to the first row values (the eigenvalues)
    sorted_eigen_params = eigen_params[:, eigen_params[0].argsort()]

    if k == -1:
        k = calc_k_using_eigengap_heuristic(sorted_eigen_params[0])

    k_first_eigen_vectors = sorted_eigen_params[1:, :k]
    return kmeans_pp(k=k, iterations=ITERATIONS, eps=EPSILON, data_points=k_first_eigen_vectors)


def main():
    if len(sys.argv) == 4:
        k = sys.argv[1]
        goal = sys.argv[2]
        file_name = sys.argv[3]
    else:
        k = -1
        goal = sys.argv[1]
        file_name = sys.argv[2]

    data_points = pd.read_csv(file_name, header=None).values.copy().tolist()

    if goal == "spk":
        result_matrix = spkmeans(data_points, k)
    else:
        result_matrix = GOAL_TO_C_MODULE_FUNCTION[goal](data_points)

    print_matrix(result_matrix)


if __name__ == '__main__':
    main()
