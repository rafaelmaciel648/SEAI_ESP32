#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// SENSORS INPUTS
#define TURB_PIN 25                 //ADC1_CH0
#define TEMP_PIN 26                 //ADC1_CH1
#define DENS_PIN 27                 //ADC1_CH2

OneWire oneWire(TEMP_PIN);                  // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature.

/**
 * @brief Setup the sensors;
 * @params
 * 
 */
void sensors_setup(void){
    sensors.begin();
}

/**
 * @brief Read value of TEMPERATURE sensor;
 * @params
 * 
 */
uint16_t temp_read(){
    uint8_t i;
    sensors.getAddress(&i, 0);
    sensors.requestTemperatures();    // Send the command to get temperatures
    return sensors.getTemp(&i);
    //return sensors.getTempC(&i));  // return in degrees celsius (float)
}

/**
 * @brief Read value of TURBIDITY sensor;
 * @params
 *
 */
uint16_t turb_read(){
    return analogRead(TURB_PIN);
}

/**
 * @brief Read value of DENSITY sensor;
 * @params
 *
 */
uint16_t dens_read(){
    return analogRead(DENS_PIN);
}