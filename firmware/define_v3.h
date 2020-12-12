
#define ROBOT_V3

#include <Wire.h>

enum dir {FORWARD = 8, BACKWARD = 2, LEFT = 4, RIGHT = 6, FRONT = 8};
enum {BEGIN, STOP, TEST, START, IDEAL,  MODE1, MODE2, MODE3, MODE4, MODE5, MODE6, MODE7};

enum {COLOR_NONE, COLOR_RED, COLOR_GREEN, COLOR_BLUE};
enum {CW, CCW};

// Not using anymore
#define ROBOT_NAME "Robot_1"

// This will enable or disable debug messages for IR communnication
#define IR_DEBUG 1

#define PIN_SERVO_RIGHT 13
#define PIN_SERVO_LEFT 12

#define PIN_DIST_SENSOR 32
#define PIN_IR_TX 16

#define PIN_SW_0 19
#define PIN_SW_1 18
#define PIN_SW_2 5

#define PIN_LED 2

#define EXPANDER_ADDRESS 0x3F
#define PIN_EXPANDER_RED 0
#define PIN_EXPANDER_GREEN 1
#define PIN_EXPANDER_BLUE 2

#define PIN_IR_LEFT 15
#define PIN_IR_RIGHT 4

byte mode = BEGIN;
byte buttonStatus = 0;
byte ROBOT_ID = 0;

String host = "192.168.8.111";

// EEPROM ---------------------------------------------------------------

#include <EEPROM.h>

#define EEPROM_SIZE 10
enum EEPROM_addresses {EEPROM_ROBOT_ID, EEPROM_LAST_STATUS, EEPROM_RIGHT_DEFAULT, EEPROM_LEFT_DEFAULT};

// NeoPixel Ring ---------------------------------------------------------------

#ifdef ENABLE_NEOPIXEL_RING
//#include "src/Robot_NeoPixel.h"
#include <Adafruit_NeoPixel.h>

#define PIN_NEOPIXEL_LED    23
#define NEOPIXEL_LED_COUNT  20
#define NEOPIXEL_BRIGHTNESS 50
#define DELAYVAL 150

Adafruit_NeoPixel strip(NEOPIXEL_LED_COUNT, PIN_NEOPIXEL_LED, NEO_GRB + NEO_KHZ800);

#endif

// Servo Motor Section ---------------------------------------------------------

#ifdef ENABLE_SERVO
#include "src/Robot_Servo.h"

Servo servoRight;
Servo servoLeft;

boolean motorState = false;
#endif

#define DEF_SPEED 50

uint8_t RIGHT_DEFAULT = 93;
uint8_t LEFT_DEFAULT = 93;//90;

#define SERVO_MIN_US 500
#define SERVO_MAX_US 2400

const float VELOCITY = 6.3; // (cm/s)

// I2C Port Expander Section ---------------------------------------------------

#ifdef ENABLE_PORTS
#include "src/Robot_PCF8574.h"
Robot_PCF8574 expander;
#endif

// Compass Section -------------------------------------------------------------

#ifdef ENABLE_COMPASS
//#include <LSM303.h>
#include "src/Robot_LSM303.h"

Robot_LSM303 compass;

// For calibration
Robot_LSM303::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = { -32768, -32768, -32768};

#endif

// Color Sensor Section --------------------------------------------------------

#ifdef ENABLE_COLOR_SENSOR
#include "src/Robot_Sensor.h"
#include "src/Robot_TCS34725.h"

byte gammatable[256];
Robot_TCS34725 tcs = Robot_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_4X);

#endif

// SharpIR Distance Sensor Section ---------------------------------------------

#ifdef ENABLE_SHARP_IR

#include "src/Robot_SharpIR.h"
Robot_SharpIR distanceSensor( Robot_SharpIR::GP2Y0A21YK0F, PIN_DIST_SENSOR);
#define DISTANCE_SENSOR_FILTER_RATE 0.99

#endif

// OTA Upload -----------------------------------------------------------------------------

#ifdef ENABLE_OTA_UPLOAD

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#endif


// Infared Communication Protocol  ---------------------------------------------------------------

#ifdef ENABLE_INFARED

// TODO: Need to check the requirements of following includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp32-hal.h"

#define IR_SENSOR_COUNT 6
#define IR_BIT_LEN 32
#define PULSE_HIGH_TICKS 20
#define PULSE_LOW_TICKS 10

rmt_obj_t* rmt_send = NULL;
rmt_obj_t* rmt_rx[IR_SENSOR_COUNT];

int flagRmtRecieve = 0;
int rmtValueReceived = 0;
int rmtValueSend = 0;

const uint8_t pin_rmtRx[IR_SENSOR_COUNT] = {39, 36, 34, 33, 27, 14};

rmt_data_t dataTx[256];
float realTickRx, realTickTx;
char sendBits[64];



#endif

// WiFi Communication API ------------------------------------------------------

#ifdef ENABLE_WIFI_MONITOR

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiMulti.h>

// For HTTP Server
WebServer wifiMonitor(80);
char tempString[100];       // Helps to build the server reply string

#endif

#ifdef ENABLE_WIFI_CLIENT

#include <HTTPClient.h>

#endif

#define WIFI_NETWORK_1

// List of available WiFi Networks
#if defined(WIFI_NETWORK_1)         // Network 1 --------------
#define WIFI_SSID "user"
#define WIFI_PASS "password"

#elif defined(WIFI_NETWORK_2)       // Network 2 --------------
#define WIFI_SSID ""
#define WIFI_PASS ""

#elif defined(WIFI_NETWORK_3)       // Network 3 --------------
#define WIFI_SSID ""
#define WIFI_PASS ""

#elif defined(WIFI_NETWORK_4)       // Network 4 --------------
#define WIFI_SSID ""
#define WIFI_PASS ""

#endif

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

// IR Communication API ------------------------------------------------------

typedef struct rmtMessage_t {
  uint16_t sourceId;
  uint16_t destinationId;
  uint16_t opCode;
  uint16_t data;
  uint32_t instruction;
} rmtMessage;


// Wheel Encoders ------------------------------------------------------


#ifdef WHEEL_ENCODER

#include "src/RobotEncoder.h"

#define PIN_ENCODER_L 4
#define PIN_ENCODER_R 15

#define ENCODER_COUNT_PER_UNIT 5

RobotEncoder enR;
RobotEncoder enL;

int32_t countL = 0, countR=0;

#endif
