#include <atomic>
#include <ctime>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>

typedef struct zone_temp
{
    std::string thermal_zone0 = "/sys/class/thermal/thermal_zone0/temp";
    std::string thermal_zone5 = "/sys/class/thermal/thermal_zone5/temp";
    std::string thermal_zone6 = "/sys/class/thermal/thermal_zone6/temp";
    std::string thermal_zone7 = "/sys/class/thermal/thermal_zone7/temp";
} zone_temp;

std::atomic<bool> active = true;

// Signal handler for Ctrl+C
void signal_handler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nTerminating program..." << std::endl;
        active = false;
    }
}

void write_to_file(std::vector<std::string>& temperature_vec, std::string&& file_path)
{
    std::ofstream out_file(file_path, std::ios::app);

    if (!out_file.is_open()) {
        exit(1);
    }

    if (!temperature_vec.empty()) {
        for (auto it = temperature_vec.begin(); it != temperature_vec.end(); ++it) {
            out_file << *it << "\n";
        }
        out_file.close();
    }
}

void read_file(std::vector<std::string>& temperatures_vec, std::string& file_path)
{
    // get current time
    std::time_t time_t { std::time(nullptr) };
    std::tm* current_time { std::localtime(&time_t) };

    // read file immedietly after opening the file. Just to make sure we record
    // time just before opening the file..
    std::ifstream in_file(file_path);
    std::string line;
    std::getline(in_file, line);
    in_file.close();

    int hours   { current_time->tm_hour };
    int minutes { current_time->tm_min };
    int seconds { current_time->tm_sec };

    if (hours == 0) {
        hours = 12;
    } else if (hours > 12) {
        hours -= 12;
    }

    std::string str_hour {
        (hours < 10 ? "0" : "") + std::to_string(hours)
    };
    std::string str_minutes {
        (minutes < 10 ? "0" : "") + std::to_string(minutes)
    };
    std::string str_seconds {
        (seconds < 10 ? "0" : "") + std::to_string(seconds)
    };

    std::string log_prefix { str_hour + ":" + str_minutes + ":" + str_seconds };

    std::string log { "[" + log_prefix + "] " + line.substr(0, 2) };
    temperatures_vec.push_back(log);
}

int main (int argc, char *argv[])
{
    std::signal(SIGINT, signal_handler);
    zone_temp thermal_zone;

    std::vector<std::string> zone_0_temps;
    std::vector<std::string> zone_5_temps;
    std::vector<std::string> zone_6_temps;
    std::vector<std::string> zone_7_temps;

    int seconds_count {0};

    while (active) {

        read_file(zone_0_temps, thermal_zone.thermal_zone0);
        read_file(zone_5_temps, thermal_zone.thermal_zone5);
        read_file(zone_6_temps, thermal_zone.thermal_zone5);
        read_file(zone_7_temps, thermal_zone.thermal_zone7);

        if (seconds_count == 60) {
            seconds_count = 0;

            write_to_file(zone_0_temps, "./thermal/thermal_zone0");
            write_to_file(zone_5_temps, "./thermal/thermal_zone5");
            write_to_file(zone_6_temps, "./thermal/thermal_zone6");
            write_to_file(zone_7_temps, "./thermal/thermal_zone7");

            zone_0_temps.clear();
            zone_5_temps.clear();
            zone_6_temps.clear();
            zone_7_temps.clear();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        seconds_count++;
    }

    write_to_file(zone_0_temps, "./thermal/thermal_zone0");
    write_to_file(zone_5_temps, "./thermal/thermal_zone5");
    write_to_file(zone_6_temps, "./thermal/thermal_zone6");
    write_to_file(zone_7_temps, "./thermal/thermal_zone7");

    zone_0_temps.clear();
    zone_5_temps.clear();
    zone_6_temps.clear();
    zone_7_temps.clear();

    return 0;
}
