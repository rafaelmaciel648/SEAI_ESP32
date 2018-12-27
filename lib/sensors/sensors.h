#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define POWER_CONTROL_PIN 14
#define POWER_TEMP_PIN 12

// SENSORS INPUTS
#define TEMP_PIN 25                 //ADC1_CH0
#define TURB_PIN 26                 //ADC1_CH1
#define DENS_PIN 27                 //ADC1_CH2

OneWire oneWire(TEMP_PIN);          // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature.

/**
 * @brief Setup the sensors;
 * @params
 * 
 */
void sensors_setup(void){
    sensors.begin();
    pinMode(POWER_CONTROL_PIN, OUTPUT);
    digitalWrite(POWER_CONTROL_PIN, LOW);
    pinMode(POWER_TEMP_PIN, OUTPUT);
    digitalWrite(POWER_TEMP_PIN, LOW);
}

/**
 * @brief Turn on sensors power
 * 
 */
void powerOn(void){
    digitalWrite(POWER_CONTROL_PIN, HIGH);
    digitalWrite(POWER_TEMP_PIN, HIGH);
}

/**
 * @brief Turn off sensors power
 * 
 */
void powerOff(void){
    digitalWrite(POWER_CONTROL_PIN, LOW);
    digitalWrite(POWER_TEMP_PIN, LOW);
}

/**
 * @brief Read value of TEMPERATURE sensor;
 * @params
 * NOTE: temp multiply by 1/128
 */
uint16_t temp_read(){
    uint8_t i;
    sensors.getAddress(&i, 0);
    sensors.requestTemperatures();      // Send the command to get temperatures
    return sensors.getTemp(&i);         // Return raw temperature value ()
    //return sensors.getTempC(&i));  // return in degrees celsius (float)
}

/**
 * @brief Read value of TURBIDITY sensor;
 * @params
 * NOTE: max value = 4095
 */
uint16_t turb_read(){
    uint8_t i;
    uint16_t avg=0;
    for(i=0; i<20; i++){
        avg = avg + analogRead(TURB_PIN);
    }
    avg = avg / 20;
    return avg;
}

/**
 * @brief Read value of DENSITY sensor;
 * @params
 * NOTE: aproxi: 200-3000
 * NOTE: 0.997 density of wine samples
 */
uint16_t dens_read(){
    return analogRead(DENS_PIN);
}