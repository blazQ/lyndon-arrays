import os
import subprocess
import random
import csv

# TODO: Save different test run in different folders

# Paths
BUILD_DIR = "build/LyndonArrays"
TEST_INPUTS_DIR = "test_inputs"
TEST_RESULTS_DIR = "test_results"

# Ensure directories exist
os.makedirs(TEST_RESULTS_DIR, exist_ok=True)

def generate_random_inputs():
    """
    Generate random test inputs and save them in the test_inputs folder.
    """
    sizes = [1000, 10000, 100000, 500000, 1000000]
    os.makedirs(TEST_INPUTS_DIR, exist_ok=True)  # Ensure input directory exists
    for size in sizes:
        file_path = os.path.join(TEST_INPUTS_DIR, f"test_file_{size}.txt")
        with open(file_path, "w") as f:
            random_string = "".join(random.choices("ab", k=size))
            f.write(random_string)

def parse_and_write_results(result, csv_writer, input_size):
    """
    Parse the timing results from the program's output and write to CSV.
    """
    output = result.stdout.strip()
    try:
        # The last line contains the timing values
        timing_line = output.split('\n')[-1]
        duration_ngs, duration_inverse = map(int, timing_line.split(','))
        csv_writer.writerow([input_size, duration_ngs, duration_inverse])
    except ValueError:
        print(f"Failed to parse timing data: {output}")

def run_test(mode, csv_file):
    """
    Run the LyndonArrays program in the specified mode and save results to a CSV.
    """
    csv_path = os.path.join(TEST_RESULTS_DIR, csv_file)
    with open(csv_path, "w", newline="") as csvfile:
        csv_writer = csv.writer(csvfile)
        csv_writer.writerow(["Input_Size", "Duration_NGS(us)", "Duration_Inverse(us)"])  # Header row

        if mode == "random":
            for size in [1000, 10000, 100000, 500000, 1000000]:
                print(f"Testing random input size: {size}")
                result = subprocess.run([BUILD_DIR, "random", str(size)], capture_output=True, text=True)
                parse_and_write_results(result, csv_writer, size)
        elif mode == "file":
            for file in os.listdir(TEST_INPUTS_DIR):
                file_path = os.path.join(TEST_INPUTS_DIR, file)
                input_size = os.path.getsize(file_path)
                print(f"Testing file: {file} ({input_size} bytes)")
                result = subprocess.run([BUILD_DIR, file_path], capture_output=True, text=True)
                parse_and_write_results(result, csv_writer, input_size)

if __name__ == "__main__":
    # Ensure test inputs exist
    generate_random_inputs()

    # Run random mode tests
    print("Running tests in random mode...")
    run_test("random", "random_mode_results.csv")

    # Run file mode tests
    print("Running tests in file mode...")
    run_test("file", "file_mode_results.csv")
