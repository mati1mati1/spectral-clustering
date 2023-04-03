import spkmeansmodule
import sys
import pandas as pd
import numpy as np 

def print_matrix(matrix_array):
    for centroid in matrix_array:
        print(",".join(("%.4f"%cord)  for cord in centroid))

def get_heuristic(eigen_values):
    gap_array = np.zeros(int(len(eigen_values)/2))
    for _ in range(int(len(eigen_values)/2)):
        gap_array[_]= abs(eigen_values[_]-eigen_values[_+1])
    
    return np.argmax(gap_array)+1

def sort_vector(eigen_vector,index_of_sorted_eigen_vector):
    sorted_eigen_vector = np.zeros_like(eigen_vector)
    for _ in range(len(eigen_vector[0])):
        sorted_eigen_vector[_] = eigen_vector[index_of_sorted_eigen_vector[_]]
    return sorted_eigen_vector

def get_index_of_sorted_eigen_vector(sorted_eigen_value,eigen_vector):
    index_of_sorted_eigen_vector = np.zeros_like(eigen_vector,dtype=int)
    for _ in range(len(eigen_vector)):
        index_of_sorted_eigen_vector[_] = int(np.where(eigen_vector==sorted_eigen_value[_])[0][0])
    return index_of_sorted_eigen_vector

def Kmeans(k,iterations,eps,data_point):

    data_point_df = pd.DataFrame(data_point) 
    merged_df_initial_size = len(data_point_df.index)
    is_natural = int(float(k)) == float(k)

    # in case k given with trailing zeroes
    k = float(k)

    is_natural = int(float(iterations)) == float(iterations)

    iterations = float(iterations)
    k = int(k)
    iterations = int(iterations)

    np.random.seed(0)

    data_point_df = data_point_df.sort_index()

    index = np.random.choice(data_point_df.index, 1)

    centroid_array = data_point_df.loc[index].to_numpy()
    data_point_df = data_point_df.drop(index)

    indexes = [str(int(index))]
    while len(centroid_array) < k:
        data_point_df["distance"] = data_point_df.apply(lambda row: min(np.linalg.norm(i - row) for i in centroid_array), axis=1)
        data_point_df["ratio"] = data_point_df["distance"] / sum(data_point_df["distance"])  

        index = np.random.choice(data_point_df.index, 1, p=data_point_df["ratio"])
        indexes.append(str(int(index)))
        
        row_to_append = data_point_df.loc[:, ~data_point_df.columns.isin(["distance", "ratio"])].loc[index].to_numpy()

        centroid_array = np.append(centroid_array, row_to_append, axis=0)

        data_point_df = data_point_df.drop(index)
        data_point_df = data_point_df.drop(["distance", "ratio"], axis=1)


    arr = centroid_array.tolist() + data_point_df.to_numpy().tolist()
    
    centroids_arr = spkmeansmodule.spk(k, 200, eps, arr)
    return centroids_arr   


def main():
    if len(sys.argv) == 4 :
        k = sys.argv[1]
        goal = sys.argv[2]
        file_name = sys.argv[3]
    else :
        k= -1
        goal = sys.argv[1]
        file_name = sys.argv[2]
    data_point = pd.read_csv(file_name,header=None).to_numpy().tolist()
    if goal == "ddg":
        matrix_array = spkmeansmodule.ddg(data_point)
    elif goal == "wam":
        matrix_array = spkmeansmodule.wam(data_point)
    elif goal == "gl":
        matrix_array = spkmeansmodule.gl(data_point)
    elif goal == "jacobi":
        matrix_array = spkmeansmodule.jacobi(data_point)
    elif goal == "spk":
        gl_matrix = spkmeansmodule.gl(data_point)
        eigen_value = np.array(spkmeansmodule.jacobi(gl_matrix))
        sorted_eigen_value = np.sort(eigen_value[0])
        sorted_index = get_index_of_sorted_eigen_vector(sorted_eigen_value,eigen_value[0])
        sorted_eigen_vector =  sort_vector(eigen_value,sorted_index)
        if k == -1:
            k = get_heuristic(sorted_eigen_vector[0])
        k_first_eigen_vector = sorted_eigen_vector[1:,:int(k)]
        print(",".join(str(index) for index in sorted_index[:int(k)]))

        matrix_array = Kmeans(int(k),300,0,k_first_eigen_vector)
    print_matrix(matrix_array)


if __name__ == '__main__':
    main()