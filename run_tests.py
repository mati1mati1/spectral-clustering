import subprocess
import os
import re
import sys
# Readme:
# 0. Make sure you have all relevant c and py files (setup.py, make, module etc...) in this directory,
#    and make sure this current file ('run_tests.py') sits in the same directory with compare_results.py and
#    all your relevant project files.
#    
# 1. In this directory, make sure you have a "tests" folder that includes:
#    ~ test_batch with all relevant tests
#    ~ ans_batch with all answers
#    ~ c_ret_batch (make sure it's empty)
#    ~ py_ret_batch (make sure it's empty)
# 
# 2. This script runs all files and outputs to relevant batch, and then compares each output to expected output.
# 
# 3. Run this script from nova terminal:
#    "python3 run_tests.py"
# 
# 4. Add -g flag to make file so valgrind can run.
# 
# 5. If you want to skip C, Python, or Valgrind:
#    add no_c / no_py / no_g flags (Respectivly)
#       
#    example:
#    "python3 run_tests.py no_g no_py" => runs test without python and without valgrind
#    GOOD LUCK


def validate_valgrind(valgrind_output, file_name):
# Parse the Valgrind output for memory leaks
    leak_pattern = re.compile(r"definitely lost: (\d+) bytes in (\d+) blocks")
    leak_matches = leak_pattern.search(valgrind_output.stderr)

    # Check if there were any memory leaks
    if leak_matches is None:
        print(f"No memory leaks detected in {file_name}!")
    else:
        bytes_lost = int(leak_matches.group(1))
        blocks_lost = int(leak_matches.group(2))
        print(f"Memory leak detected: {bytes_lost} bytes in {blocks_lost} blocks in {file_name}")

def main(no_py, no_c, no_g):
    print("Running tests")
    print("\n##########################\n")
    print("Setting up & validating folders:\n")
    isExit = False
    if not os.path.exists("tests"):
            print(f"Missing 'tests' folder")
            exit(1)
    if not os.path.exists('tests/test_batch'):
            print(f"Missing tests_batch folder in tests")
            isExit = True
    if not os.path.exists('tests/ans_batch'):
            print(f"Missing ans_batch folder in tests")
            isExit = True 
    if isExit:
        exit(1)
    else:
        print("All folders set.\n")


    c_folder_path = "tests/c_ret_batch"
    py_folder_path = "tests/py_ret_batch"
    for folder in [c_folder_path, py_folder_path]:
        if not os.path.exists(folder):
            os.makedirs(folder)
            print(f"The folder {folder} was created in the tests directory.")
    if not (no_c and no_g):
        print("C part:\n")
        print("Makefile...")
        command = "make"
        result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
        if result.stderr != "":
            print("Error in Make...")
            print(result.stderr)
            exit(1)
        print(result.stdout)
    # Run C part: wam, ddg, gl, jacobi
    if not no_c:
        for filename in os.listdir('tests/test_batch'):
            if filename.endswith('_j.txt'):
                print(f"Running jacobi in {filename.split('_')[0]}")
                command = f"./spkmeans jacobi tests/test_batch/{filename} > tests/c_ret_batch/{filename.split('.')[0]}_ans.txt"
                subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
            else:
                for goal in ['wam', 'ddg', 'gl']:
                    print(f"Running {goal} in {filename.split('.')[0]}")
                    command = f"./spkmeans {goal} tests/test_batch/{filename} > tests/c_ret_batch/{filename.split('.')[0]}_{goal}.txt"
                    subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
    else: 
        print("skipping C")
    print("Finished running C part")
    print("\n##########################\n")
    if not no_py:
        print("Py part:\n")
        print("Setting up...")
        command = 'python3 setup.py build_ext --inplace'
        result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
        print(result.stdout)
        # Run Py part: wam, ddg, gl, jacobi, spk
        for filename in os.listdir('tests/test_batch'):
            filepath = os.path.join('tests/test_batch', filename)
            if filename.endswith('_j.txt'):
                print(f"Running jacobi in {filename.split('_')[0]}")
                command = f'python3 spkmeans.py jacobi tests/test_batch/{filename} > tests/py_ret_batch/{filename.split(".")[0]}_ans.txt'
                subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
            else:
                for goal in ['wam', 'ddg', 'gl', 'spk']:
                    print(f"Running {goal} in {filename.split('_')[0]}")
                    command = f'python3 spkmeans.py {goal} tests/test_batch/{filename} > tests/py_ret_batch/{filename.split(".")[0]}_{goal}.txt'            
                    subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
    else:
        print("Skipping Py")
    print("Finished running Py part")
    print("\n##########################\n")

    if not no_c:
        print("Comparing C part:\n")
        command = f'python3 compare_results.py tests/ans_batch tests/c_ret_batch'
        result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
        output = result.stdout
        print(output)

    if not no_py:
        print("Comparing Py part:\n")
        command = f'python3 compare_results.py tests/ans_batch tests/py_ret_batch'
        result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
        output = result.stdout
        print(output)

    if not no_g:
        print("Valgrinding...")
        for filename in os.listdir('tests/test_batch'):
            if filename.endswith('_j.txt'):
                print(f"Valgrinding jacobi in {filename.split('_')[0]}")
                command = f"valgrind ./spkmeans jacobi tests/test_batch/{filename}"
                valgrind_output = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
                validate_valgrind(valgrind_output, filename)
            else:
                for goal in ['wam', 'ddg', 'gl']:
                    print(f"Valgrinding {goal} in {filename.split('.')[0]}")
                    command = f"valgrind ./spkmeans {goal} tests/test_batch/{filename}"
                    valgrind_output = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
                    validate_valgrind(valgrind_output, filename)
    else:
        print("Skipping Valgrind")


if __name__ == "__main__":
    no_py = True if  'no_py' in sys.argv else False
    no_c = True if 'no_c' in sys.argv else False
    no_g = True if 'no_g' in sys.argv else False         
    main(no_py, no_c, no_g)