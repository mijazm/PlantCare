class Plant{
    public:
        const char* plant_name;//!<Name of the plant
        unsigned int moisture_sensor_pin;
        double soil_moisture; //soil mosisture measured by capacitive sensor

        double min_soil_moisture;
        double max_soil_moisture;

        unsigned int temp_sensor_pin;
        double soil_temperature; //Soil temperature
        unsigned int watering_state;// 1. If plant is getting watered, 0. If Not

        Plant(const char* _plant_name, unsigned int _moisture_sensor_pin, unsigned int _temp_sensor_pin){

            plant_name  = _plant_name;
            moisture_sensor_pin = _moisture_sensor_pin;
            temp_sensor_pin = _temp_sensor_pin;
            watering_state = 0;
            min_soil_moisture = 20.0;
            max_soil_moisture = 60.0;

        }
        void set_target_moisture(double _min_soil_moisture, double _max_soil_moisture);
        void setup_sensors();
        double get_soil_moisture();
        double read_soil_temp();
        void monitor();
};