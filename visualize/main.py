import matplotlib.pyplot as plt
from datetime import datetime

# Read the log file
log_filename = "../thermal/thermal_zone0"
times = []
temperatures = []

with open(log_filename, "r") as file:
    for line in file:
        time_str, temperature_str = line.strip().split()
        time = datetime.strptime(time_str[1:-1], "%H:%M:%S")
        temperature = int(temperature_str)
        times.append(time)
        temperatures.append(temperature)

# Plot the data using a line graph without markers
plt.figure(figsize=(12, 6))

plt.plot(times, temperatures, linestyle='-')

plt.title('CPU Temperature Over Time')
plt.xlabel('Time')
plt.ylabel('Temperature')
plt.grid(True)

plt.show()
