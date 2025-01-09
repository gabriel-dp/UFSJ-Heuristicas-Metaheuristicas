import seaborn as sns
import matplotlib.pyplot as plt
import sys

if len(sys.argv) < 3:
    print("Usage: python results.py <filename> <algorithm>")
    sys.exit(1)
file_path = sys.argv[1]
algorithm = sys.argv[2]

try:
    with open(file_path, "r") as file:
        lines = file.readlines()

    data = []
    for line in lines:
        try:
            data.append(float(line.strip()))
        except ValueError:
            continue

    if len(data) > 1:
        data = data[:-1]

    iterations = list(range(1, len(data) + 1))
    sns.set(style="whitegrid")

    plt.figure(figsize=(10, 6))
    sns.lineplot(x=iterations, y=data, color="blue", linewidth=0.1)  # Reduced line thickness

    plt.title(f"Progresso do algoritmo {algorithm}", fontsize=16)
    plt.xlabel("Solução aceita", fontsize=12)
    plt.ylabel("Função Objetivo", fontsize=12)
    plt.xticks(fontsize=12)
    plt.yticks(fontsize=12)

    # Adjust both axis start
    plt.xlim(left=0)
    plt.ylim(bottom=426)

    output_file = "progress.png"
    plt.tight_layout()
    plt.savefig(output_file)
    print(f"Plot saved as {output_file}")

except FileNotFoundError:
    print(f"Error: File '{file_path}' not found.")
except Exception as e:
    print(f"An error occurred: {e}")
