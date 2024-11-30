#include <vector>
#include <unordered_map>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Config{
private:
    size_t frequency_;
    size_t days_counter_;
    std::vector<std::string> cities_;
public:
    [[nodiscard]] size_t frequency() const{
        return frequency_;
    }

    [[nodiscard]] size_t days_counter() const{
        return days_counter_;
    }

    std::vector<std::string>& cities(){
        return cities_;
    }

    void set_frequency(int n){
        frequency_ += n / 1000;
    }

    void Add_Argument(std::string& key, std::string& value){
        if(key == "frequency"){
            frequency_ = static_cast<size_t>(std::atoi(value.c_str()));
        }
        if(key == "days_counter"){
            days_counter_ = static_cast<size_t>(std::atoi(value.c_str()));
        }
        if(key == "cities_list"){
            std::string city_name;
            for(size_t i = 1; i < value.size(); i++){
                char c = value[i];
                if(c != ','){
                    if(c == '}'){
                        cities_.push_back(city_name);
                    } else {
                        if(isspace(c)){
                            city_name += "%20";
                        } else {
                            city_name += c;
                        }
                    }
                } else {
                    cities_.push_back(city_name);
                    city_name = "";
                    i++;
                }
            }
        }
    }

    void Config_Parse(std::ifstream& in){
        int cnt = 0;
        while(!in.eof()){
            char c;
            std::string key;
            std::string value;
            while(true){
                in.get(c);
                if(!isspace(c)){
                    key += c;
                } else {
                    in.get(c);
                    in.get(c);
                    break;
                }
            }
            while(true){
                in.get(c);
                if(c != '\r'){
                    value += c;
                } else {
                    in.get(c);
                    break;
                }
            }
            Add_Argument(key, value);
            cnt++;
            if(cnt >= 3) break;
        }
    }
};

struct Weather_Data{
    int weather_code;
    double temperature;
    int wind_direction;
    int wind_speed;
    int precipitation_probability;
    double precipitation;
    std::string date;
};

class Weather_Forecast {
private:
    std::vector<Weather_Data> forecast;

public:
    void set(const json& data);
    void print(uint8_t days);
};

std::string city_print(const std::string& city);

cpr::Response Ninja_Request(std::string& city_name);
cpr::Response Open_Meteo_Request(std::string& latitude, std::string& longitude);
json City_Request(std::string& city);

void update(json y, Weather_Forecast& x, int curr, size_t days, Config config);
