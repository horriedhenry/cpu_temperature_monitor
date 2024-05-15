#include <iostream>
#include <ctime>

// Log : [time] temperature

int main (int argc, char *argv[])
{
    std::time_t currentTime { std::time(nullptr) };
    std::tm* timeInfo { std::localtime(&currentTime) };

    int hours {timeInfo->tm_hour};
    int minutes {timeInfo->tm_min};
    int seconds {timeInfo->tm_sec};

    if (hours == 0) {
        hours = 12;
    } else if (hours > 12) {
        hours -= 12;
    }

    std::cout << hours << ":" 
        << (minutes < 10 ? "0" : "") << minutes << ":"
        << (seconds < 10 ? "0" : "") << seconds << ""
        << std::endl;

    return 0;
}
