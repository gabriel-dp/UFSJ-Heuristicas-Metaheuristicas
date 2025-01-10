import sys
import matplotlib.pyplot as plt

def plot_solution(file_path, plot_title, target_value):
    # Initialize lists for X and Y values
    x_vals = []
    y_vals = []

    # Read the file and process each line
    with open(file_path, 'r') as file:
        for line in file:
            # Split the line on the basis of '|' and extract the solution values
            parts = line.strip().split('|')
            # Extract the Y value (the last part in parentheses)
            y_value = float(parts[-1].strip()[1:-1])  # Removing the parentheses
            x_vals.append(len(x_vals) + 1)  # X is the line number
            y_vals.append(y_value)

    # Create a scatter plot for the solutions
    plt.scatter(x_vals, y_vals, color='blue', label='Soluções')
    
    # Plot the target value as a red dashed line
    plt.axhline(y=target_value, color='red', linestyle='--', label=f'Solução ótima ({target_value})')

    # Adding title and labels
    plt.title(plot_title)
    plt.xlabel('Execução')
    plt.ylabel('Resultado')
    
    # Adding a legend
    plt.legend()

    # Show the plot
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Uso correto: python script.py <file_path> <plot_title> <target_value>")
    else:
        file_path = sys.argv[1]
        plot_title = sys.argv[2]
        target_value = float(sys.argv[3])
        plot_solution(file_path, plot_title, target_value)
