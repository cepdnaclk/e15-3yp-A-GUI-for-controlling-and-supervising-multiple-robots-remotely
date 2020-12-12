int head = 0;

void loop() {

  switch (mode) {
    case BEGIN:

      buttonStatus = (!digitalRead(PIN_SW_1) * 2) + (!digitalRead(PIN_SW_0));

      //Serial.println(buttonStatus);
      //delay(1000);

      if (buttonStatus == 1) {
        Serial.println("Mode1");
        mode = MODE1;

      } else if (buttonStatus == 2) {
        Serial.println("Mode2");
        mode = MODE2;

      } else if (buttonStatus == 3) {
        Serial.println("Mode3");
        mode = MODE3;

      } else if (buttonStatus == 4) {
        Serial.println("Mode4");
        mode = MODE4;

      } else if (buttonStatus == 5) {
        Serial.println("Mode5");
        mode = MODE5;

      } else if (buttonStatus == 6) {
        Serial.println("Mode6");
        mode = MODE6;

      } else if (buttonStatus == 7) {
        Serial.println("Mode7");
        mode = MODE7;
      } else {
        delay(100);
      }

      // Call the functions which should execute on every loop cycle,  this is available only on MODE_0
      loopEvents();

      break;

    // ------------------------------------------------------------------------------------------ Mode 1 ----------
    case MODE1:
      //motorTest();
      motorWrite(200, 200);
      delay(7000);

      motorStop();

      mode = BEGIN;

      break;

    // ------------------------------------------------------------------------------------------ Mode 2 ----------
    case MODE2:
      // What should happen on mode 2
      colorSensorTest();
      break;

    // ------------------------------------------------------------------------------------------ Mode 3 ----------
    case MODE3:
      // What should happen on mode 3
      neoPixelTest();

      break;

    // ------------------------------------------------------------------------------------------ Mode 4 ----------
    case MODE4:
      // What should happen on mode 4

      break;

    // ------------------------------------------------------------------------------------------ Mode 5 ----------
    case MODE5:
      // What should happen on mode 5
      //sendTestSignal();

      /*if ((flagRmtRecieve != 0) && (rmtValueReceived != 0)) {
        Serial.printf("flagRmtRecieve=%d, value=%d\n", flagRmtRecieve, rmtValueReceived); delay(100);
        rmtMessage_t current = messageDecode(rmtValueReceived);
        //instructionExecution(rmtValueReceived);
        protocolHandshake_Execution(current);
        delay(1000);
        flagRmtRecieve = 0;
        rmtValueReceived = 0;

        }*/
      break;

    // ------------------------------------------------------------------------------------------ Mode 6 ----------
    case MODE6:
      // What should happen on mode 6

      for (int i = 0; i < 2; i++) {
        //motorWrite(50, 50);
        //motorStop();
        //delay(2500);
        turnRight(90);
        delay(2000);
      }
      for (int i = 0; i < 2; i++) {
        //motorWrite(50, 50);
        //motorStop();
        //delay(2500);
        turnLeft(90);
        delay(2000);
      }

      break;

    // ------------------------------------------------------------------------------------------ Mode 7 ----------
    case MODE7:
      // What should happen on mode 7
      delay(100);

      break;
  }
}

void fun() {
  int leftIr = analogRead(15);
  int rightIr = analogRead(4);
  Serial.printf("left-%d right-%d\n", leftIr, rightIr);
  delay(100);


}

void testCompassAlign0() {
  int diff = 0;
  int a0 = getAngleInt();
  int a = a0;
  Serial.printf("  starting=%d \n", a0);
  motorWrite(5, 5);

  while (1) {

    a = getAngleInt();

    if (abs(a - a0) > 2) {
      diff += (a - a0); // integrate the difference
    }


    Serial.printf("  diff: (a:%d - a0:%d) = diff:%d\n", a, a0, abs(diff));

    if (abs(diff) > 100) { // Correct only when a huge difference
      Serial.printf("  aligning @ %d to %d \n", diff, a0);
      comapssAlign(a0);
      motorWrite(0, 0);
      delay(1000);
      blink();
      diff = 0;
      motorWrite(3, 2);
    }
    delay(25);
  }
}

void testCompassAlign() {
  int startAngle = getAngleInt();

  while (1) {
    motorWrite(50, 50);
    delay(5000);
    comapssAlign(startAngle);
  }
}
void comapssAlign() {
  // with pre defined value
  comapssAlign(160);
}

int diffAngle(int a0, int a1) {
  return (a1 - a0); // - (((a1 - a0) > 180) ? 180 : 0);
}
void comapssAlign(int alignTo) {

  // REM: Need to consider about the situation getAngle=359 and alignTo=5
  int diff = diffAngle(alignTo, getAngleInt());

  // Use to avoid oscillations
  //int n = 0;

  if ( abs(diff) > 2) { // Threshold [-2,2]

    while ((abs(diff) > 2) ) { // && n < 500
      Serial.printf(" comapssAlign: diff= %d \n", diff);
      if (diff > 0) {
        motorWrite(-3, 2);    // (diff>0) CCW
      } else {
        motorWrite(3, -2);    // (diff<0) CW
      }
      diff = diffAngle(alignTo, getAngleInt());
      //n++;
    }

  } else {
    Serial.println("Matching");
  }
  delay(50);
}

/*void fun() {
  Serial.println()
  /*motorWrite(50, 50);
    float d = getDistance();
    Serial.println(d);
    if (d <= 10) {
    delay(200);
    motorWrite(0, 0);
    delay(2000);
    Serial.println(">> Broadcasting a handshake request");
    sendWaveform(messageEncode(ROBOT_ID, NODE_BROADCAST_ID , OPCODE_HANDSHAKE , DATA_HANDSHAKE_REQ), 32);
    //sendWaveform(messageEncode(ROBOT_ID, NODE_BROADCAST_ID , OPCODE_HANDSHAKE , DATA_HANDSHAKE_ACK2), 32);
    mode = 50;
    // }
    }
  }*/

void loopEvents() {

  // Handle OTA requests, if it is enabled
#ifdef ENABLE_OTA_UPLOAD
  ArduinoOTA.handle();
#endif

  // WiFi Monitor : Only support on MODE_0
#ifdef ENABLE_WIFI_MONITOR
  wifiMonitor.handleClient();
#endif

}

void blink() {
  digitalWrite(PIN_LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(PIN_LED, LOW);    // turn the LED off by making the voltage LOW
  delay(500);
}
