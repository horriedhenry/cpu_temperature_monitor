#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#define file_zone0 "./thermal/thermal_zone0"
#define file_zone5 "./thermal/thermal_zone5"
#define file_zone6 "./thermal/thermal_zone6"
#define file_zone7 "./thermal/thermal_zone7"

int main (int argc, char *argv[])
{
    std::ifstream in_file(file_zone0);
    std::map<int, int> temp_log;

    if (!in_file.is_open()) {
        exit(1);
    }
    
    std::string line;
    std::vector<int> temps;
    while (std::getline(in_file, line)) {
        temps.push_back(std::stoi(line));
    }

    for (auto it = temps.begin(); it != temps.end(); ++it) {
        temp_log[*it]++;
    }

    int pair_ = 0;
    for (const auto& pair_count : temp_log) {
        std::cout << pair_count.first << " : " << pair_count.second << std::endl;
        pair_ += pair_count.second;
    }
    std::cout << "temps size : " << temps.size() << std::endl;
    std::cout << "pair_count : " << temps.size() << std::endl;
    return 0;
}
