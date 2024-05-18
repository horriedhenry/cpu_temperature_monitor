import matplotlib.pyplot as plt
from datetime import datetime, timedelta
import matplotlib.dates as mdates

def read_log_file(filename):
    temperatures_dict = {}
    with open(filename, "r") as file:
        for line in file:
            time_str, temperature_str = line.strip().split()
            time = datetime.strptime(time_str[1:-1], "%H:%M:%S")
            temperature = int(temperature_str)
            temperatures_dict[time] = temperature
    
    # Create a list of all timestamps in the range
    start_time = min(temperatures_dict.keys())
    end_time = max(temperatures_dict.keys())
    all_times = [start_time + timedelta(seconds=x) for x in range((end_time - start_time).seconds)]
    
    # Interpolate missing temperatures
    temperatures = [temperatures_dict[time] if time in temperatures_dict else None for time in all_times]
    
    return all_times, temperatures

filenames = [
    "../thermal/thermal_zone0",
    "../thermal/thermal_zone5",
    "../thermal/thermal_zone6",
    "../thermal/thermal_zone7"
]

current_zone_index = 0

fig, ax = plt.subplots(figsize=(10, 6))
times, temperatures = read_log_file(filenames[current_zone_index])
ax.plot(times, temperatures, linestyle='None', marker='o')
ax.set_title(f'CPU Temperature Over Time ({filenames[current_zone_index].split("/")[-1]})')
ax.set_xlabel('Time')
ax.set_ylabel('Temperature')
ax.grid(True)

# Format the x-axis to show time properly
ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M'))
fig.autofmt_xdate()

# navigate through graphs
def on_key(event):
    global current_zone_index
    if event.key == 'right':
        current_zone_index = (current_zone_index + 1) % len(filenames)
    elif event.key == 'left':
        current_zone_index = (current_zone_index - 1) % len(filenames)
    times, temperatures = read_log_file(filenames[current_zone_index])
    ax.clear()
    ax.plot(times, temperatures, linestyle='None', marker='o')
    ax.set_title(f'CPU Temperature Over Time ({filenames[current_zone_index].split("/")[-1]})')
    ax.set_xlabel('Time')
    ax.set_ylabel('Temperature')
    ax.grid(True)
    ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M'))
    fig.autofmt_xdate()
    plt.draw()

# Connect the event handler
fig.canvas.mpl_connect('key_press_event', on_key)

plt.show()
