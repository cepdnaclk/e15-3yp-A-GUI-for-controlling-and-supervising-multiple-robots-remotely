
/*
  Constructor methods
  --- beginSwitches();
  --- beginMagSensor();
  --- beginAccelSensor();
  --- beginColorSensor();
  --- beginDistanceSensor();

  Test Methods
  --- magSensorTest();
  --- accelSensorTest();
  --- colorSensorTest();
  --- distanceSensorTest();

  Functions
  --- getColorCode();

  Information Methods
  --- displayMagSensorDetails();
  --- displayAccelSensorDetails();
*/

void beginSwitches() {
  pinMode(PIN_SW_0, INPUT_PULLUP);
  pinMode(PIN_SW_1, INPUT_PULLUP);
  pinMode(PIN_SW_2, INPUT_PULLUP);

  // IR Sensors
  pinMode(PIN_IR_LEFT, INPUT_PULLDOWN);
  pinMode(PIN_IR_RIGHT, INPUT_PULLDOWN);
}


#ifdef WHEEL_ENCODER
void beginEncoders() {
  Serial.println(">> begin: TCS34725");
  enR.setCount(0);
  enL.setCount(0);

  enR.attachCounter(PIN_ENCODER_R);
  enL.attachCounter(PIN_ENCODER_L);
}
void encoderPrint() {
  Serial.printf("Encoder L:%d R:%d\n", enL.getCount(), enR.getCount());
  delay(100);
}

void encoderReset(){
   enL.clearCount();
   enR.clearCount();
}
int encoderAvg(){
   return (enL.getCount() + enR.getCount())/2;
}

#else
void beginEncoders() {}
void encoderPrint() {}
void encoderReset() {}
#endif

// *** Color Sensor ************************************************************

#ifdef ENABLE_COLOR_SENSOR

void beginColorSensor() {
  if (tcs.begin()) Serial.println(">> begin: TCS34725");
  else Serial.println(">> error: TCS34725 (not found)");
  generateGammaTable();
  tcs.setInterrupt(true);  // turn off LED
}
void generateGammaTable() {
  // it helps convert RGB colors to what humans see
  for (int i = 0; i < 256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;
    gammatable[i] = x;
    //Serial.println(gammatable[i]);
  }
}

void colorSensorTest() {

  uint16_t red, green, blue, c,  colorTemp, lux;
  float r, g, b;

  tcs.setInterrupt(false);      // turn on LED
  delay(60);

  tcs.getRawData(&red, &green, &blue, &c);
  colorTemp = tcs.calculateColorTemperature(red, green, blue);
  lux = tcs.calculateLux(red, green, blue);

  tcs.setInterrupt(true);  // turn off LED

  r = (int)((red * 256) / (float)c);
  g = (int)((green * 256) / (float)c);
  b = (int)((blue * 256) / (float)c);

#ifdef ENABLE_NEOPIXEL_RING
  pixelShowColor(gammatable[(int)r], gammatable[(int)g], gammatable[(int)b]);
#endif

  Serial.printf("Color Sensor\tR:%d G:%d B:%d temp:%d lux:%d\n", (int)r, (int)g, (int)b, c, colorTemp, lux);
  Serial.println(getColorCode());

}
uint8_t getColorCode() {

  delay(500);
  int flag = 1;

  while ((flag > 0) && (flag < 1000)) { // exit if 1000; avoid infinity loop

    uint16_t r, g, b, c, colorTemp, lux;

    tcs.setInterrupt(false);      // turn on LED
    delay(60);

    tcs.getRawData(&r, &g, &b, &c);
    //colorTemp = tcs.calculateColorTemperature(r, g, b);
    //lux = tcs.calculateLux(r, g, b);
    tcs.setInterrupt(true);  // turn off LED

    if (r > g && r > b) {
      Serial.println("RED");
      flag = 0;
      return COLOR_RED;
    }
    else if (g > r && g > b) {
      Serial.println("GREEN");
      flag = 0;
      return COLOR_GREEN;
    }
    else if (b > r && b > g) {
      Serial.println("BLUE");
      flag = 0;
      return COLOR_BLUE;
    }
    delay(25);
    flag++;
  }
  return COLOR_NONE;
}

#else

void beginColorSensor() {
  Serial.println(">> info: TCS34725 (not enabled)");
}
void generateGammaTable() {}
void colorSensorTest() {}
uint8_t getColorCode() {
  return COLOR_NONE;
}

#endif

// *** Compass + Accelerometer Sensor ******************************************

#ifdef ENABLE_COMPASS

void beginMagSensor() {
  Wire.begin();
  compass.init();
  compass.enableDefault();

  compass.m_min = (Robot_LSM303::vector<int16_t>) {
    -524,   -547,   -407
  };
  compass.m_max = (Robot_LSM303::vector<int16_t>) {
    +328,   +327,   +282
  };
}

float getAngle() {
  float currentAngle = 0;
  float preAngle = 0;
  float error = 0;
  uint8_t count = 0;

  //while (count < 1000) { // to avoid infinity loop
  compass.read();
  float heading = compass.heading();

  /*if (count == 0) {
    preAngle = heading;
    } else {
    currentAngle = heading;
    error = abs(currentAngle - preAngle);

    if (error < 10) return ((currentAngle + preAngle) / 2);
    preAngle = currentAngle;
    }

    delay(10);
    ++count;
    }*/

  // return currentAngle;
  return heading;

}

int getAngleInt() {
  float heading = 0.0;

  compass.read();
  heading = compass.heading();

  //for (int i = 0; i < 10; i++) {
  //}
  return (int)(heading);
}

/*void magSensorTest() {
  sensors_event_t event;
  mag.getEvent(&event);

  Serial.printf("magnetic\tx:%f y:%f z:%f uT\n", event.magnetic.x, event.magnetic.y, event.magnetic.z);
  //Serial.printf("magnetic_raw\tx:%f y:%f z:%f uT\n", mag.raw.x, mag.raw.y, mag.raw.z);
  delay(500);

  }*/
void accelSensorTest() {

  /*sensors_event_t event;
    //accel.getEvent(&event);

    Serial.printf("acceleration\tx:%f y:%f z:%f m/s^2\n", event.acceleration.x, event.acceleration.y, event.acceleration.z);
    //Serial.printf("acceleration_raw\tx:%f y:%f z:%f m/s^2\n", accel.raw.x, accel.raw.y, accel.raw.z);
    delay(500);*/
}

#else

void beginMagSensor() {
  Serial.println(">> info: LSM303_Mag (not enabled)");
}
void beginAccelSensor() {
  Serial.println(">> info: LSM303_Accel (not enabled)");
}

void magSensorTest() {}
void accelSensorTest() {}
float getAngle() {
  return 0;
}
int getAngleInt() {
  return 0;
}
#endif


#ifdef ENABLE_SHARP_IR
void beginDistanceSensor() {
  Serial.println("begin: SharpIRSensor");
  distanceSensor.setFilterRate(DISTANCE_SENSOR_FILTER_RATE);
}
float getDistance() {

  float distance = 0;

  for (int i = 0; i < 10; i++) {
    distance = distanceSensor.getDistanceFloat();
  }
  // Print raw distance in cm
  //Serial.println(distanceSensor.getRawDistance());

  // Print distance in cm
  //Serial.println(distanceSensor.getDistance());

  // Print distance in cm, as a float value
  //Serial.println(distanceSensor.getDistanceFloat());

  return distance;
}
void distanceSensorTest() {
  // Print raw distance in cm
  //Serial.println(distanceSensor.getRawDistance());

  // Print distance in cm
  //Serial.println(distanceSensor.getDistance());

  // Print distance in cm, as a float value
  Serial.printf(" raw:%f float:%f int:%f\n", distanceSensor.getRawDistance(), distanceSensor.getDistanceFloat(), distanceSensor.getDistance());
}
#else
void beginDistanceSensor() {
  Serial.println(">> info: SharpIRSensor (not enabled)");
}
void getDistance() {}
void distanceSensorTest() {}
#endif


// -----------------------------------------------------------------

// Need to update followings

float readInputVoltage() {

  int reading = analogRead(35);
  float average = 0;
  float alpha = 0.1f;

  // Read 10 samples
  for (int i = 0; i < 10; i++) {
    average = (alpha * reading) + (1 - alpha) * average;
  }

  float voltage = 0;
  float analogReadArray[] = {720, 703, 677, 655, 635, 615, 590, 573, 555, 531, 505, 480};
  float voltageArray[] = {4.10, 4.00, 3.90, 3.80, 3.70, 3.60, 3.50, 3.40, 3.30, 3.20, 3.10, 3.00};

  if (average > analogReadArray[0])
    voltage = voltageArray[0];

  else if (average < analogReadArray[11])
    voltage = voltageArray[11];

  else {
    for (int i = 0; i < 12; ++i) {
      if (average == analogReadArray[i]) {
        voltage = voltageArray[i];
        break;
      }
      else if (average > analogReadArray[i]) {
        voltage = (voltageArray[i] + voltageArray[i - 1]) / 2;
        break;
      }
    }
  }
  //Serial.printf("%d %d\n", reading, (int)average);
  Serial.printf("%f ===> %f\n", average, voltage);
  delay(5000);
}









// -----------------------------------------------------------------------------

#ifdef ENABLE_COMPASS
/*void displayMagSensorDetails() {
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
  }
  void displayAccelSensorDetails() {
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
  }*/
#else
void displayMagSensorDetails(void) {}
void displayAccelSensorDetails(void) {}
#endif
