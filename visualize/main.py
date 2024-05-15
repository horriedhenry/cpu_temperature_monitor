import matplotlib.pyplot as plt
import numpy as np

thermal_zone0 = "../thermal/thermal_zone0"
thermal_zone5 = "../thermal/thermal_zone5"
thermal_zone6 = "../thermal/thermal_zone6"
thermal_zone7 = "../thermal/thermal_zone7"

def zone(thermal_zone, zone_number):

    with open(thermal_zone, mode="r") as zone0_data:
        temperatures = [int(line.strip()) for line in zone0_data.readlines()]

    # Plotting the histogram
    plt.figure(figsize=(10, 5))
    plt.subplot(1, 2, 1)
    plt.hist(temperatures, bins=20, color="blue", alpha=0.7)
    plt.xlabel("Temperature")
    plt.ylabel("Frequency")
    plt.title(f"Temperature Frequency Distribution {zone_number}")
    plt.grid(True)

    # Generating example time data
    time = np.arange(0, len(temperatures))

    # Plotting the scatter plot
    plt.subplot(1, 2, 2)
    plt.scatter(
        time, temperatures, label="Temperature vs Time", color="green", alpha=0.7
    )

    # Fit a polynomial trend line (degree 1) to the data
    trend_coefficients = np.polyfit(time, temperatures, 1)
    trend_line = np.poly1d(trend_coefficients)
    plt.plot(
        time,
        trend_line(time),
        color="red",
        linestyle="--",
        label="Trend Line (Degree 1)",
    )

    # Set labels and title
    plt.xlabel("Time")
    plt.ylabel("Temperature")
    plt.title(f"Temperature vs Time Scatter Plot with Trend Line {zone_number}")
    plt.legend()

    plt.tight_layout()

    # Show the plot
    plt.grid(True)
    plt.show()


zone(thermal_zone0, "zone0")
zone(thermal_zone5, "zone5")
zone(thermal_zone6, "zone6")
zone(thermal_zone7, "zone7")
