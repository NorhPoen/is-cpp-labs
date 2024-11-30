#include <iostream>
#include <cmath>

#include "DataRequests.h"

#include "ConsoleTable/CppConsoleTable.hpp"
#include <nlohmann/json.hpp>

using ConsoleTable = samilton::ConsoleTable;
using json = nlohmann::json;

cpr::Response Ninja_Request(std::string& city_name){
    std::string url = "https://api.api-ninjas.com/v1/city?name=" + city_name;
    std::string api_key = "8G3lbZUopFDOW/JEFHkCgg==9ua6HxvvpV8nfSz3";

    cpr::Header header {
            {"X-Api-Key", api_key}
    };

    cpr::Response x = cpr::Get(cpr::Url{url}, header);

    return x;
}

cpr::Response Open_Meteo_Request(std::string& latitude, std::string& longitude){
    std::string url = "https://api.open-meteo.com/v1/forecast?latitude=" + latitude + "&longitude=" + longitude
                              + "&hourly=temperature_2m," + "relativehumidity_2m," + "precipitation_probability,"
                              + "precipitation," + "weathercode," + "surface_pressure," + "windspeed_10m," + "winddirection_10m";

    cpr::Header header {
            {"X-Api-Key", "api_key"}
    };

    cpr::Response x = cpr::Get(cpr::Url{url}, header);

    return x;
}

json City_Request(std::string& city){
    cpr::Response cpr = Ninja_Request(city);

    json x = json::parse(cpr.text);

    double latitude = x[0]["latitude"];
    double longitude = x[0]["longitude"];

    std::string latitude_str = std::to_string(latitude);
    std::string longitude_str = std::to_string(longitude);

    cpr::Response cpr2 = Open_Meteo_Request(latitude_str, longitude_str);

    json y = json::parse(cpr2.text);

    return y;
}

Weather_Data get_info(const json& data, int day, int id){
    double temperature = 0;
    int wind_direction = 0;
    int wind_speed = 0;
    int precipitation_probability = 0;
    double precipitation = 0;
    for(int j = id; j < id + 6; j++){
        temperature += (double)data["hourly"]["temperature_2m"][24 * day + j];
        wind_direction += (int)data["hourly"]["winddirection_10m"][24 * day + j];
        wind_speed += (int)data["hourly"]["windspeed_10m"][24 * day + j];
        precipitation_probability += (int)data["hourly"]["precipitation_probability"][24 * day + j];
        precipitation += (double)data["hourly"]["precipitation"][24 * day + j];
    }
    Weather_Data weather_data{};
    weather_data.date = data["hourly"]["time"][24 * day];
    weather_data.date.erase(weather_data.date.begin() + 10, weather_data.date.begin() + 16);
    weather_data.weather_code = (int)data["hourly"]["weathercode"][24 * day + id + 3];
    weather_data.temperature = temperature / 6;
    weather_data.wind_direction = wind_direction / 6;
    weather_data.wind_speed = wind_speed / 6;
    weather_data.precipitation_probability = precipitation_probability / 6;
    weather_data.precipitation = precipitation / 6;
    weather_data.temperature = std::round(weather_data.temperature * 10.0) / 10.0;
    weather_data.precipitation = std::round(weather_data.precipitation * 10.0) / 10.0;

    return weather_data;
}

void Weather_Forecast::set(const json& data) {
    forecast.clear();
    for(int i = 0; i < 7; i++){
        forecast.push_back(get_info(data, i, 0));
        forecast.push_back(get_info(data, i, 6));
        forecast.push_back(get_info(data, i, 12));
        forecast.push_back(get_info(data, i, 18));
    }
}

std::string city_print(const std::string& city){
    std::string tmp;
    for(size_t i = 0; i < city.size(); i++){
        if(city[i] == '%'){
            i += 2;
            tmp += " ";
        } else {
            tmp += city[i];
        }
    }
    return tmp;
}

std::vector<std::string> weather_art(const std::string& weather_type){
    if(weather_type == "Cloudy") {
        return {"                ",
                "      .--.      ",
                "   .-(    ).    ",
                "  (___.__)__)   ",
                "                "};
    }
    if(weather_type == "Fog"){
        return {"   _  _  -  _   ",
                "  _ - _ _  - -  ",
                "   -_ - _  - _  ",
                "   _   -   _    ",
                "               "};
    }
    if(weather_type == "Snow"){
        return {"      .-.       ",
                "     (   ).     ",
                "    (___(__)    ",
                "    * * * *     ",
                "   * * * *      "};
    }
    if(weather_type == "Rain"){
        return {"      .-.       ",
                "     (   ).     ",
                "    (___(__)    ",
                "    ' '' ''     ",
                "     ' '' '     ",};
    }
    if(weather_type == "Storm"){
        return {"      .-.       ",
                "     (   ).     ",
                "    (___(__)    ",
                "     _/ /       ",
                "    /_/         ",
                "   /            ",};
    }
    return {"      .      ",
            "    | | /    ",
            "  '-.;;;.-'  ",
            " -==;;;;;==- ",
            "  .-';;;'-.  ",
            "    / | |    ",
            "      '      "};
}

std::string weather_type(int code){
    if(code == 0){
        return "Clear sky";
    }
    if(code < 4){
        return "Cloudy";
    }
    if(code == 45 || code == 48){
        return "Fog";
    }
    if((code >= 71 && code <= 77) || code == 85 || code == 86){
        return "Snow";
    }
    if(code >= 95){
        return "Storm";
    }
    return "Rain";
}

std::string wind_direction(int value){
    if ((value >= 0 && value <= 22) || (value >= 338 && value <= 360)) {
        return "N ";
    }
    if (value >= 23 && value <= 67) {
        return "NE ";
    }
    if (value >= 68 && value <= 112) {
        return "E ";
    }
    if (value >= 113 && value <= 157) {
        return "SE ";
    }
    if (value >= 158 && value <= 202) {
        return "S ";
    }
    if (value >= 203 && value <= 247) {
        return "SW ";
    }
    if (value >= 248 && value <= 292) {
        return "W ";
    }
    return "NW ";
}

void Weather_Forecast::print(uint8_t days) {
    char degreeSymbol = 248;
    for(size_t i = 0; i < 4 * days; i += 4) {
        ConsoleTable table(1, 1, samilton::Alignment::centre);
        ConsoleTable table_date(1, 1, samilton::Alignment::centre);
        table[0][0] = "Night";
        table[0][1] = "Morning";
        table[0][2] = "Afternoon";
        table[0][3] = "Evening";

        for(int j = 0; j < 4; j++) {
            table[1][j] = weather_type(forecast[i + j].weather_code) +
                    '\n' + std::to_string((int)forecast[i + j].temperature) + " " + degreeSymbol + "C" +
                    '\n' + wind_direction(forecast[i + j].wind_direction) +
                    std::to_string(forecast[i + j].wind_speed) + " km/h" + '\n'
                    + std::to_string(forecast[i + j].precipitation) + " mm | " +
                    std::to_string(forecast[i + j].precipitation_probability) + '%' + '\n';
        }

        std::vector<std::string> art = weather_art(weather_type(forecast[i + 12].weather_code));
        table_date[0][0] = forecast[i].date;

        std::cout << table_date << '\n';
        for(int j = 0; j < art.size(); j++){
            std::cout << art[j] << '\n';
        }                                                                          

        std::cout << table << '\n';
    }
}

void update(json y, Weather_Forecast& x, int curr, size_t days, Config config){
    cpr::Response cpr = Ninja_Request(config.cities()[curr]);

    if(cpr.text.size() < 10 || cpr.status_code != 200){
        std::cout << "Something is wrong, try later or check connection.\n";
        return;
    }

    y = City_Request(config.cities()[curr]);
    x.set(y);
    system("cls");
    std::cout << city_print(config.cities()[curr]) << '\n';
    x.print(days);
}
