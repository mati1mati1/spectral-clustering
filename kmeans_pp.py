import pandas as pd
import sys
import numpy as np
import mykmeanssp as kmeans

DEBUG = False

def dprint(string): 
    if DEBUG:
        print(string)

def print_centroids(centroids_arr):
    for centroid in centroids_arr:
        print(",".join(("%.4f"%cord)  for cord in centroid))

def Kmeans(k,iterations,eps,input_file_name_1,input_file_name_2):

    first_df = pd.read_csv(input_file_name_1, index_col=0, header=None)
    second_df = pd.read_csv(input_file_name_2, index_col=0, header=None)
    merged_df = pd.merge(first_df, second_df, left_index=True, right_index=True) 
    merged_df_initial_size = len(merged_df.index)
    try:
        is_natural = int(float(k)) == float(k)
    except ValueError:
        print("Invalid number of clusters!")
        return

    if not is_natural:
        print("Invalid number of clusters!")
        return

    # in case k given with trailing zeroes
    k = float(k)
    if k <= 1 or k >= merged_df_initial_size:
        print("Invalid number of clusters!")
        return

    try:
        is_natural = int(float(iterations)) == float(iterations)
    except ValueError:
        print("Invalid maximum iteration!")
        return
    if not is_natural:
        print("Invalid maximum iteration!")
        return

    iterations = float(iterations)
    if iterations <= 1 or iterations >= 1000:
        print("Invalid maximum iteration!")
        return
    k = int(k)
    iterations = int(iterations)

    np.random.seed(0)

    merged_df = merged_df.sort_index()

    index = np.random.choice(merged_df.index, 1)
    dprint(f'chosen index is: {index}')

    centroid_array = merged_df.loc[index].to_numpy()
    dprint(f'centroids array:\n{centroid_array}')
    merged_df = merged_df.drop(index)

    indexes = [str(int(index))]
    while len(centroid_array) < k:
        merged_df["distance"] = merged_df.apply(lambda row: min(np.linalg.norm(i - row) for i in centroid_array), axis=1)
        merged_df["ratio"] = merged_df["distance"] / sum(merged_df["distance"])  
        dprint("-" * 10)

        index = np.random.choice(merged_df.index, 1, p=merged_df["ratio"])
        indexes.append(str(int(index)))
        dprint(f"index: {index}")
        
        row_to_append = merged_df.loc[:, ~merged_df.columns.isin(["distance", "ratio"])].loc[index].to_numpy()

        centroid_array = np.append(centroid_array, row_to_append, axis=0)

        merged_df = merged_df.drop(index)
        merged_df = merged_df.drop(["distance", "ratio"], axis=1)

        dprint(f'updated:\n{centroid_array}')

    arr = centroid_array.tolist() + merged_df.to_numpy().tolist()

    if DEBUG:
        assert(len(arr) == merged_df_initial_size)

    print(",".join(indexes))
    centroids_arr = kmeans.fit(k, 200, eps, arr)
    print_centroids(centroids_arr)



def main():
    if(len(sys.argv) == 6):
        Kmeans(sys.argv[1],sys.argv[2],float(sys.argv[3]),sys.argv[4],sys.argv[5])
    elif(len(sys.argv) == 5):
        Kmeans(sys.argv[1],300,float(sys.argv[2]),sys.argv[3],sys.argv[4])
    else:
        print("An Error Has Occurred")

if __name__ == "__main__":
    main()
