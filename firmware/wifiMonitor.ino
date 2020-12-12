#ifdef ENABLE_WIFI_MONITOR

void beginWiFiMonitor() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  pixelColorWave(0, 50, 0); // Green

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  delay(1000);
  pixelOff();

  //if (MDNS.begin("esp32")) { Serial.println("MDNS responder started"); }

  wifiMonitor.on("/", handleRoot);
  wifiMonitor.on("/status", robotStatus);
  
  wifiMonitor.onNotFound(handleNotFound);

  // JSON APIs
  wifiMonitor.on("/mag", getMag);
  wifiMonitor.on("/accel", getAccel);
  wifiMonitor.on("/dist", getDist);
  wifiMonitor.on("/color", getColor);

  wifiMonitor.on("/motor", handleMotion);
  wifiMonitor.on("/turn", handleRotation);

  wifiMonitor.on("/pixelLED/all", handlePixelLED);
  wifiMonitor.on("/eeprom/write", handleEEPROM);
  wifiMonitor.on("/ir/send", handleIR);
  
  wifiMonitor.on("/path", handleMotionPath);
  

  wifiMonitor.begin();
  Serial.print(">> WiFi Monitor: on\n");
}

// --- Robot as a Client -------------------------------------------------------

void httpPrint(String input) {
  //if ((wifiMulti.run() == WL_CONNECTED)) {
  IPAddress ip =  WiFi.localIP();
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");

  http.begin("192.168.43.182", 3000, "/post");
  http.addHeader("Content-Type", "application/json");

  sprintf(tempString, "{\"robotIP\":\"%d.%d.%d.%d\",\"text\":\"%s\"}", ip[0], ip[1], ip[2], ip[3], input);
  int httpResponseCode = http.POST(tempString); //Send the actual POST request

  // start connection and send HTTP header
  if (httpResponseCode > 0) {
    String response = http.getString();                       //Get the response to the request
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }

  http.end();  //Free resources
  //} else {
  //   Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  //}

  http.end();
}


// --- Robot as a Server -------------------------------------------------------
void handleRoot() {
  String message = "Root Directory\n\n";
  wifiMonitor.send(200, "text/plain", message);
}
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += wifiMonitor.uri();
  message += "\nMethod: ";
  message += (wifiMonitor.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += wifiMonitor.args();
  message += "\n";

  for (uint8_t i = 0; i < wifiMonitor.args(); i++) {
    message += " " + wifiMonitor.argName(i) + ": " + wifiMonitor.arg(i) + "\n";
  }

  wifiMonitor.send(404, "text/plain", message);
}

// General Monitor Functions
void robotStatus() {

  IPAddress ip =  WiFi.localIP();
  sprintf(tempString, "{\"status\":\"Active\",\"version\":\"Swarm_v2\",\"IP\":\"%d.%d.%d.%d\"}", ip[0], ip[1], ip[2], ip[3]);

  // REM: Neet to test this code
  if (wifiMonitor.args() == 1) {
    // update server IP
    host = wifiMonitor.arg(0);
    Serial.println(host);
  }

  wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
  wifiMonitor.send(200, "application/json", tempString);
}

// API Level Functions
void getMag() {

#ifdef ENABLE_COMPASS
  compass.read();
  float heading =  compass.heading();
  //if (heading < 0 )heading += 360;

  Serial.printf("magnetic\tx:%d y:%d z:%d uT\t %f", compass.m.x, compass.m.y, compass.m.z, heading);
  sprintf(tempString, "{\"x\": \"%d\", \"y\": %d, \"z\": \"%d\", \"heading\": \"%f\"}", compass.m.x, compass.m.y, compass.m.z, heading);
  wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
  wifiMonitor.send(200, "application/json", tempString);

#else

  Serial.print("magnetic\tN/A");
  sprintf(tempString, "{\"x\": \"%d\", \"y\": %d, \"z\": \"%d\", \"heading\": \"%f\"}", 0, 0, 0, 0);
  wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
  wifiMonitor.send(200, "application/json", tempString);

#endif
}
void getAccel() {

#ifdef ENABLE_COMPASS
  compass.read();

  // Format into m/s^2

  float ax = (float)compass.a.x; //* 0.001F * SENSORS_GRAVITY_STANDARD;
  float ay = (float)compass.a.y; //* * 0.001F * SENSORS_GRAVITY_STANDARD;
  float az = (float)compass.a.z; //* * 0.001F * SENSORS_GRAVITY_STANDARD;

  compass.read();
  Serial.printf("acceleration\tx:%f y:%f z:%f  m/s^2\n", ax, ay, az);
  sprintf(tempString, "{\"x\": \"%f\", \"y\": %f, \"z\": \"%f\"}", ax, ay, az);
  wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
  wifiMonitor.send(200, "application / json", tempString);

#else
  Serial.print("acceleration\tN/A");
  sprintf(tempString, "{\"x\": \"%d\", \"y\": %d, \"z\": \"%d\"}", 0, 0, 0);
  wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
  wifiMonitor.send(200, "application / json", tempString);

#endif

}
void getDist() {

#ifdef ENABLE_SHARP_IR
  int rawDistance = distanceSensor.getDistanceFloat();
  float filteredDistance = distanceSensor.getDistanceFloat();

  Serial.printf("Distance\traw:%d filtered:%f \n", rawDistance, filteredDistance);
  sprintf(tempString, "{\"raw\": \"%d\", \"filtered\": \"%f\"}", rawDistance, filteredDistance);
  wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
  wifiMonitor.send(200, "application / json", tempString);

#else

  Serial.print("Distance\tN/A");
  sprintf(tempString, "{\"raw\": \"%d\", \"filtered\": \"%f\"}", 0, 0);
  wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
  wifiMonitor.send(200, "application / json", tempString);

#endif
}
void getColor() {

#ifdef ENABLE_COLOR_SENSOR

  uint16_t red, green, blue, c,  colorTemp, lux;

  tcs.setInterrupt(false);      // turn on LED
  delay(60);

  tcs.getRawData(&red, &green, &blue, &c);
  colorTemp = tcs.calculateColorTemperature(red, green, blue);
  lux = tcs.calculateLux(red, green, blue);

  tcs.setInterrupt(true);  // turn off LED

  /*
    Serial.print("C:\t"); Serial.print(c);
    Serial.print("\tR:\t"); Serial.print(red);
    Serial.print("\tG:\t"); Serial.print(green);
    Serial.print("\tB:\t"); Serial.println(blue);
  */

  float r, g, b;

  r = (red * 256) / (float)c;
  g = (green * 256) / (float)c;
  b = (blue * 256) / (float)c;

  // Only for test
  //pixelShowColor(gammatable[(int)r], gammatable[(int)g], gammatable[(int)b]);

  Serial.printf("Color Sensor\tR:%d G:%d B:%d temp:%d lux:%d\n", (int)r, (int)g, (int)b, c, colorTemp, lux);
  sprintf(tempString, "{\"R\": \"%d\", \"G\": \"%d\", \"B\": \"%d\" , \"temp\": \"%d\" , \"lux\": \"%d\"}", (int)r, (int)g, (int)b, colorTemp, lux);
  wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
  wifiMonitor.send(200, "application / json", tempString);

#else

  Serial.print("Color Sensor\tN/A");
  sprintf(tempString, "{\"R\": \"%d\", \"G\": %d, \"B\": \"%d\" , \"temp\": \"%d\" , \"lux\": \"%d\",\"code\" : \"%d\"}", 0, 0, 0, 0, 0, 0);
  wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
  wifiMonitor.send(200, "application / json", tempString);

#endif
}

void handleMotion() {
  wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
  int spd = 200;

  if (wifiMonitor.args() > 0) {

    if (wifiMonitor.args() > 1) {
      spd = wifiMonitor.arg(1).toInt();
    }

    if (wifiMonitor.arg(0).compareTo("stop") == 0) {
      Serial.println("motor/stop");
      motorStop();

    } else if (wifiMonitor.arg(0).compareTo("forward") == 0) {
      Serial.println("motor/forward");
      motorWrite(spd, spd);

    } else if (wifiMonitor.arg(0).compareTo("backward") == 0) {
      Serial.println("motor/backward");
      motorWrite(-1*spd, -1*spd);

    } else if (wifiMonitor.arg(0).compareTo("rotCW") == 0) {
      Serial.println("motor/rotCW");
      motorWrite(spd, -1*spd);

    } else if (wifiMonitor.arg(0).compareTo("rotCCW") == 0) {
      Serial.println("motor/rotCCW");
      motorWrite(-1*spd, spd);

    } else {
      Serial.println("motor/stop");
      motorStop();
    }

    sprintf(tempString, "{\"status\":\"success\"}");
    wifiMonitor.send(200, "application/json", tempString);

  } else {
    wifiMonitor.send(200, "text / plain", "No direction received");
  }

}
void handleRotation() {

  wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");

  if (wifiMonitor.args() > 0) {
    float angle = wifiMonitor.arg(0).toFloat();
    Serial.print("motor/rotate");
    Serial.println(angle);

    sprintf(tempString, "{\"status\":\"success\",\"angle\":\"%f\"}", angle); //   wifiMonitor.arg(0)
    wifiMonitor.send(200, "application/json", tempString);
    turn(angle);

  } else {
    wifiMonitor.send(200, "text / plain", "No direction received");
  }

}
void handlePixelLED() {
  uint8_t r, g, b;

  if (wifiMonitor.args() > 0) {

    r = wifiMonitor.arg(0).toInt();
    g = wifiMonitor.arg(1).toInt();
    b = wifiMonitor.arg(2).toInt();
    pixelShowColor(r, g, b);
  }
  sprintf(tempString, "{\"status\":\"N/A\"}");
  wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
  wifiMonitor.send(200, "application/json", tempString);

}

void handleEEPROM() {
  if (wifiMonitor.args() > 0) {
    Serial.printf("eepromWrite(%d,%d);\n", wifiMonitor.arg(0).toInt(), wifiMonitor.arg(1).toInt());
  }

  sprintf(tempString, "{\"status\":\"N/A\"}");
  wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
  wifiMonitor.send(200, "application/json", tempString);

}
void handleIR() {
  if (wifiMonitor.args() > 0) {
    Serial.printf("irSend(%s);\n", wifiMonitor.arg(0).toInt(), wifiMonitor.arg(0));
  }

  sprintf(tempString, "{\"status\":\"N/A\"}");
  wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
  wifiMonitor.send(200, "application/json", tempString);

}

void handleMotionPath() {
  // Server gives the path to follow
  String message = wifiMonitor.arg(0);              //Get the value of the parameter
  Serial.println(message);
  sprintf(tempString, "{\"status\":\"success\"}"); //   wifiMonitor.arg(0)
  wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
  wifiMonitor.send(200, "application/json", tempString);

  followPath(message);
}



#else

void beginWiFiMonitor() {
  Serial.println(">> info: WiFiMonitor (not enabled)");
}

void robotStatus() {}
void getAngleToMonitor() {}
void getColorToMonitor() {}
void getDistanceToMonitor() {}

void getMag() {}
void getAccel() {}
void getDist() {}
void getColor() {}

void handleMotion() {}
void handleRotation() {}
void handlePixelLED() {}

void handleMotionPath() {}
#endif
