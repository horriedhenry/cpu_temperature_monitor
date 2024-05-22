#include <iostream>
#include <ctime>
#include <map>
#include <sstream>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>

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

    std::vector<std::string> temp_rec_zone0;
    std::vector<std::string> temp_rec_zone5;
    std::vector<std::string> temp_rec_zone6;
    std::vector<std::string> temp_rec_zone7;

    std::map<std::string, std::vector<std::string>*> temperature_records {
        { "./thermal/thermal_zone0" , &temp_rec_zone0 },
        { "./thermal/thermal_zone5" , &temp_rec_zone5 },
        { "./thermal/thermal_zone6" , &temp_rec_zone6 },
        { "./thermal/thermal_zone7" , &temp_rec_zone7 },
    };

    void write_temperature_records()
    {
        std::ofstream file;
        std::stringstream str_stream;

        for (auto itr = temperature_records.begin(); itr != temperature_records.end(); ++itr) {
            for (const auto& str : *itr->second) {
                str_stream << str << "\n";
            }

            file.open(itr->first, std::ios::app);
            if (file.is_open()) {
                file << str_stream.str();
            } else {
                return;
            }
            file.close();
            str_stream.str("");
            str_stream.clear();
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

    for (int i = 0; i < 10; i++) {
        temps.read_file(temps.file_paths[0], temps.temp_rec_zone0);
        temps.read_file(temps.file_paths[1], temps.temp_rec_zone5);
        temps.read_file(temps.file_paths[2], temps.temp_rec_zone6);
        temps.read_file(temps.file_paths[3], temps.temp_rec_zone7);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    temps.write_temperature_records();

    return 0;
}
