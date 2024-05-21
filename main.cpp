#include <iostream>
#include <ctime>

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

int main (int argc, char *argv[])
{
    std::time_t time_t{std::time(nullptr)};
    std::tm* tm {std::localtime(&time_t)};
    Time time {get_time(tm)};

    std::string curr_time {time.time()};
    std::cout << curr_time << std::endl;
    return 0;
}
