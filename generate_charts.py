import os
import pandas as pd
import matplotlib.pyplot as plt

# Paths
TEST_RESULTS_DIR = "test_results"
CHARTS_DIR = os.path.join(TEST_RESULTS_DIR, "charts")

# Ensure charts directory exists
os.makedirs(CHARTS_DIR, exist_ok=True)

def generate_chart(csv_file, chart_file_1, chart_file_2, title_1, title_2, x_label, y_label_1, y_label_2):
    """Generate two charts from a CSV file, one for the computation time of the Next Greater Suffix Array and one for the Inverse Lyndon Array."""
    csv_path = os.path.join(TEST_RESULTS_DIR, csv_file)
    chart_path_1 = os.path.join(CHARTS_DIR, chart_file_1)
    chart_path_2 = os.path.join(CHARTS_DIR, chart_file_2)

    # Load data
    data = pd.read_csv(csv_path)

    # Plot chart for NGS execution time
    plt.figure(figsize=(10, 6))
    plt.plot(data["Input_Size"], data["Duration_NGS(us)"], marker="o", label="NGS Execution Time")
    plt.title(title_1)
    plt.xlabel(x_label)
    plt.ylabel(y_label_1)
    plt.grid(True)
    plt.legend()

    # Save the first chart for NGS
    plt.savefig(chart_path_1)
    print(f"Chart saved to {chart_path_1}")
    plt.clf()  # Clear the figure to avoid overlaying

    # Plot chart for Inverse Lyndon execution time
    plt.figure(figsize=(10, 6))
    plt.plot(data["Input_Size"], data["Duration_Inverse(us)"], marker="o", label="Inverse Lyndon Execution Time")
    plt.title(title_2)
    plt.xlabel(x_label)
    plt.ylabel(y_label_2)
    plt.grid(True)
    plt.legend()

    # Save the second chart for Inverse Lyndon
    plt.savefig(chart_path_2)
    print(f"Chart saved to {chart_path_2}")
    plt.clf()  # Clear the figure for the next plot

if __name__ == "__main__":
    # Generate charts for random mode
    print("Generating charts for random mode results...")
    generate_chart(
        "random_mode_results.csv",
        "random_mode_ngs_chart.png",  # NGS chart
        "random_mode_inverse_chart.png",  # Inverse Lyndon chart
        "Random Mode - NGS Execution Time",
        "Random Mode - Inverse Lyndon Execution Time",
        "Input Size (characters)",
        "NGS Execution Time (us)",
        "Inverse Lyndon Execution Time (us)"
    )

    # Generate charts for file mode
    print("Generating charts for file mode results...")
    generate_chart(
        "file_mode_results.csv",
        "file_mode_ngs_chart.png",  # NGS chart
        "file_mode_inverse_chart.png",  # Inverse Lyndon chart
        "File Mode - NGS Execution Time",
        "File Mode - Inverse Lyndon Execution Time",
        "Input Size (bytes)",
        "NGS Execution Time (us)",
        "Inverse Lyndon Execution Time (us)"
    )
