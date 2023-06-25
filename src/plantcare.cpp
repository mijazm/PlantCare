#include <plantcare.h>
#include <Arduino.h>

//Update these values after caliberation of soil moisture sensor
#define dry 3000
#define wet 1700

void Plant::set_target_moisture(double _min_soil_moisture, double _max_soil_moisture)
{
    min_soil_moisture = _min_soil_moisture;
    max_soil_moisture = _max_soil_moisture;
}

void Plant::setup_sensors()
{
    pinMode(moisture_sensor_pin,INPUT);
}

/// @brief 
/// @return 
double Plant::get_soil_moisture(){
    auto soil_moisture_raw = analogRead(moisture_sensor_pin);
    soil_moisture = map(soil_moisture_raw,dry,wet,0,100);
    return soil_moisture_raw;
}

void Plant::monitor()
{
    if(soil_moisture < min_soil_moisture){
        watering_state = 1;
    }
    if(soil_moisture > max_soil_moisture){
        watering_state = 0;
    }
}
