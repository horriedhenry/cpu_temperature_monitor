#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>

typedef struct Time
{
    std::string hr;
    std::string mn;
    std::string sc;
    std::string am_pm;
    std::string time() const
    {
        return hr + ":" + mn + ":" + sc + am_pm;
    }
} Time;

Time get_time(std::tm* tm)
{
    Time time;
    int tm_hr {tm->tm_hour};
    int tm_mn {tm->tm_min};
    int tm_sc {tm->tm_sec};

    time.am_pm = (tm_hr < 12) ? " AM" : " PM";

    if (tm_hr == 0) {
        tm_hr = 12;
    } else if (tm_hr > 12) {
        tm_hr -= 12;
    }

    time.hr = (tm_hr < 10 ? "0" : "") + std::to_string(tm_hr);
    time.mn = (tm_mn < 10 ? "0" : "") + std::to_string(tm_mn);
    time.sc = (tm_sc < 10 ? "0" : "") + std::to_string(tm_sc);
    return time;
}

class Temps
{
public:
    std::vector<std::string> file_paths {
        "/sys/class/thermal/thermal_zone0/temp",
        "/sys/class/thermal/thermal_zone5/temp",
        "/sys/class/thermal/thermal_zone6/temp",
        "/sys/class/thermal/thermal_zone7/temp",
    };

    std::vector<std::string> temps_zone0;
    std::vector<std::string> temps_zone5;
    std::vector<std::string> temps_zone6;
    std::vector<std::string> temps_zone7;

    std::vector<std::vector<std::string>*> temperature_zones {
        &temps_zone0,
        &temps_zone5,
        &temps_zone6,
        &temps_zone7,
    };

    void print_temps()
    {
        for (auto vec_ptr : temperature_zones) {
            for (auto it = vec_ptr->begin(); it != vec_ptr->end(); ++it) {
                std::cout << *it << " ";
            }
            std::cout << std::endl;
        }
    }

    void read_file(std::string& file_path, std::vector<std::string>& vec_temps)
    {
        std::ifstream file(file_path);
        std::string line;
        if (file.is_open()) {
            std::getline(file, line);
            vec_temps.push_back(line.substr(0, 2));
        } else {
            return;
        }
        file.close();
    }
};

int main (int argc, char *argv[])
{
    Temps temps;

    temps.read_file(temps.file_paths[0], temps.temps_zone0);
    temps.read_file(temps.file_paths[1], temps.temps_zone5);
    temps.read_file(temps.file_paths[2], temps.temps_zone6);
    temps.read_file(temps.file_paths[3], temps.temps_zone7);

    temps.print_temps();
    return 0;
}
