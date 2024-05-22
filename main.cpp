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
    std::vector<std::string> temp_rec_zone0;
    std::vector<std::string> temp_rec_zone5;
    std::vector<std::string> temp_rec_zone6;
    std::vector<std::string> temp_rec_zone7;

    std::map<std::string, std::vector<std::string>*> temperature_records_in {
        { "/sys/class/thermal/thermal_zone0/temp" , &temp_rec_zone0 },
        { "/sys/class/thermal/thermal_zone5/temp" , &temp_rec_zone5 },
        { "/sys/class/thermal/thermal_zone6/temp" , &temp_rec_zone6 },
        { "/sys/class/thermal/thermal_zone7/temp" , &temp_rec_zone7 },
    };

    std::map<std::string, std::vector<std::string>*> temperature_records_out {
        { "./thermal/thermal_zone0" , &temp_rec_zone0 },
        { "./thermal/thermal_zone5" , &temp_rec_zone5 },
        { "./thermal/thermal_zone6" , &temp_rec_zone6 },
        { "./thermal/thermal_zone7" , &temp_rec_zone7 },
    };

    void write_temperature_records()
    {
        std::ofstream file;
        std::stringstream str_stream;

        for (auto itr = temperature_records_out.begin(); itr != temperature_records_out.end(); ++itr) {
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

    std::string log(std::string&& log_value, std::string&& time)
    {
        std::string log {"[" + time + "] " + log_value};
        return log;
    }

    void read_thermal_zones()
    {
        std::ifstream file;
        std::string line;

        std::time_t time_t {std::time(nullptr)};
        std::tm* tm {std::localtime(&time_t)};

        for (auto itr = temperature_records_in.begin(); itr != temperature_records_in.end(); ++itr) {
            file.open(itr->first);
            if (file.is_open()) {
                std::getline(file, line);
                Time time = get_time(tm);

                itr->second->push_back(log(line.substr(0, 2), time.time()));

                line.clear();
                file.close();
            } else {
                return;
            }
        }
    }
};

int main (int argc, char *argv[])
{
    Temps temps;
    int seconds_count{0};

    while (true) {
        temps.read_thermal_zones();

        if (seconds_count == 60) {
            temps.write_temperature_records();
            seconds_count = 0;
            return 0;
        }

        seconds_count++;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}
