#include <iostream>
#include <fstream>
#include <conio.h>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "ConsoleTable/CppConsoleTable.hpp"

#include "DataRequests.h"

using json = nlohmann::json;
using ConsoleTable = samilton::ConsoleTable;

int main(int argc, char* argv[]) {
    Config config;
    std::ifstream in("Config.txt", std::ios::binary);
    config.Config_Parse(in);

    Weather_Forecast x;
    size_t days = config.days_counter();
    int curr = 0;

    json y = City_Request(config.cities()[curr]);
    std::cout << city_print(config.cities()[curr]) << '\n';
    x.set(y);
    x.print(days);

    while (true) {
        int command = getch();
        if (command == 27) {
            break;
        }
        if (command == 77) {
            curr = std::min(curr + 1, (int)config.cities().size() - 1);
        }
        if (command == 75) {
            curr = std::max(curr - 1, 0);
        }
        if (command == 72) {
            if(days != 7){
                days++;
            }
        }
        if (command == 80) {
            if(days != 1){
                days--;
            }
        }
        if(clock() >= 1000 * config.frequency()){
            update(y, x, curr, days, config);
            config.set_frequency(clock());
        }
    }

    return 0;
}
