import os
import sys
# Define the paths to the two folders
def main(folder1_path, folder2_path):
    # Get a list of all the files in folder1
    folder1_files = os.listdir(folder1_path)
    errCnt = 0
    # Loop over the files in folder1
    for file in folder1_files:
        # Check if the file exists in folder2
        if os.path.exists(os.path.join(folder2_path, file)):
            # Read the contents of the files in both folders
            with open(os.path.join(folder1_path, file), "r") as f1, open(os.path.join(folder2_path, file), "r") as f2:
                folder1_content = f1.read()
                folder2_content = f2.read()
            # Compare the contents of the two files
            if folder1_content != folder2_content:
                errCnt += 1
                print(f"{file} has different content in both folders")

    print(f"Total errors: {errCnt}")

if __name__ == "__main__":
    main(sys.argv[1], sys.argv[2])