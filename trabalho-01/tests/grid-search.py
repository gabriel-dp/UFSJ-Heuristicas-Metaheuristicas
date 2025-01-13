import subprocess
import itertools
import time
import re

# Define your file paths
executable_file_path = "./bin/grasp-kp"
input_file_path = "data/kp.txt"

# Define your test setup
iterations = 1

# Define your parameter ranges
param1_values = [10, 100, 1000, 10000]
param2_values = [0.0625, 0.125, 0.25, 0.5, 0.75, 0.875, 0.9375] 

# Generate all parameter combinations
parameter_combinations = list(itertools.product(param1_values, param2_values))

# Iterate over all parameter combinations
results = []
for params in parameter_combinations:
    # Test the parameter combination multiple times
    param_results = []
    for i in range(iterations):
        with open(input_file_path, "r") as input_file:
            start_time = time.time()
        
            command = [executable_file_path] + list(map(str, params))
            result = subprocess.run(
                command,
                stdin=input_file,        # Pass file handle as stdin
                stdout=subprocess.PIPE,  # Capture standard output
                stderr=subprocess.PIPE,  # Capture standard error
                text=True,               # Ensure the output is returned as a string
            )

            execution_time = time.time() - start_time

            # Extract value from the output using regex
            match = re.search(r"\((.*?)\)", result.stdout)
            if match:
                value = float(match.group(1))  # Convert to float if numeric
                param_results.append((value, execution_time))

    # Calculate averages
    if param_results:
        avg_value = sum(res[0] for res in param_results) / len(param_results)
        avg_time = sum(res[1] for res in param_results) / len(param_results)
        results.append((params, avg_value, avg_time))

# Print the results
print("Average Results:")
for params, avg_value, avg_time in results:
    print(f"Parameters: {params}, Average Value: {avg_value:.2f}, Average Time: {avg_time:.4f} seconds")
