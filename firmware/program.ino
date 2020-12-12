
//#define DRIVE_SERVO
#define DRIVE_PWM

#if defined(DRIVE_SERVO)
#define ENABLE_SERVO                      // Enable motors (required 6kB space)
#elif defined(DRIVE_PWM)
#define ENABLE_PWM                        // Enable motors with PWM controller (required xkB space)
#endif

#define ENABLE_COLOR_SENSOR               // Enable TCS34725 Color sensor (required 2kB space)
#define ENABLE_COMPASS                  // Enable GY-511 compass + accelerometer (required 3kB space)
#define ENABLE_SHARP_IR                 // Enable GP2Y0A21YK0F analog distance sensor, GP2Y0A21YK0F (required 2kB space)
//#define ENABLE_PORTS                    // Enable PCF8564 I2C port expander (required 1kB space)
#define ENABLE_OTA_UPLOAD               // Enable or disable OTA (On the air upload via WiFi, required 531kB space)
#define ENABLE_INFARED                  // Enable IR Transmiter and receiver (required 1kB space)
#define ENABLE_WIFI_MONITOR             // Enable Wifi Monitor
#define ENABLE_NEOPIXEL_RING            // Enable NeoPixel Ring
#define ENABLE_WIFI_CLIENT   // REM: Some dependancy with WiFi Monitor
#define WHEEL_ENCODER

#include "Arduino.h"

//#include "define_v3.h"   // Configurations for version 3 PCB
#include "define_v4.h"   // Configurations for version 4 PCB

void setup() {
   Serial.begin(115200);
   Serial.println(">> begin");
   pinMode(PIN_LED, OUTPUT);

   displayMemoryDetails();
   beginSwitches();
   beginEEPROM();

   beginLED();
   pixelColorWave(0, 0, 50);

   motorSetup();

   beginMagSensor();
   //beginAccelSensor();
   beginColorSensor();
   beginDistanceSensor();
   beginInfared();

   beginWiFiMonitor();
   beginOTA();
   beginEncoders();

   pixelOff();

}

void displayMemoryDetails() {
   Serial.printf("The program uses %d kB and has free space of %d kB\n\n", ESP.getSketchSize() / 1000, ESP.getFreeSketchSpace() / 1000);
}

#ifdef ENABLE_INFARED

// Interrupt for Remote Rx Event
extern "C" void irReceive_0(uint32_t *data, size_t len) {
   parseRmtData((rmt_data_t*) data, len, 0);
}
extern "C" void irReceive_1(uint32_t *data, size_t len) {
   parseRmtData((rmt_data_t*) data, len, 1);
}

extern "C" void irReceive_2(uint32_t *data, size_t len) {
   parseRmtData((rmt_data_t*) data, len, 2);
}
extern "C" void irReceive_3(uint32_t *data, size_t len) {
   parseRmtData((rmt_data_t*) data, len, 3);
}

#if IR_SENSOR_COUNT==6
extern "C" void irReceive_4(uint32_t *data, size_t len) {
   parseRmtData((rmt_data_t*) data, len, 4);
}
extern "C" void irReceive_5(uint32_t *data, size_t len) {
   parseRmtData((rmt_data_t*) data, len, 5);
}
#endif

#endif

void beginEEPROM() {

   EEPROM.begin(EEPROM_SIZE);

   //writeEEPROM(EEPROM_ROBOT_ID, 2);  // This will assign an unique ID for every robot

   // Should write into Robot 1
   //writeEEPROM(EEPROM_LEFT_DEFAULT, 93);
   //writeEEPROM(EEPROM_RIGHT_DEFAULT, 93);

   // Load default data
   ROBOT_ID = EEPROM.read(EEPROM_ROBOT_ID);
   //RIGHT_DEFAULT = EEPROM.read(EEPROM_RIGHT_DEFAULT);
   //LEFT_DEFAULT = EEPROM.read(EEPROM_LEFT_DEFAULT);

   Serial.printf("RobotID: %d\n", ROBOT_ID);
}

void writeEEPROM(uint16_t address, uint8_t data) {
   // address: [0-511], data: [0-255]
   EEPROM.write(address, data);
   EEPROM.commit();
}
