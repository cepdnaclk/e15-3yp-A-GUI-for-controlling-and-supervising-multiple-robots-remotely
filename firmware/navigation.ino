
//new

void followPath(String path) {

   char m;
   for (int i = 0; i < path.length(); ++i)
   {
      Serial.printf("At a junction\ni: %d \n",  i);
      m = path.charAt(i);
      if (m == 'F') {
         //encoderForward(5);
      }
      else if (m == 'R') {
         //turnRight();
         delay(500);
      }
      else if (m == 'L') {
         //turnLeft();
         delay(500);
      }
      else {
         motorStop();
      }
   }
}


//------------------------------

/*
int alignToMaze() {
   int left = digitalRead(PIN_IR_LEFT);
   int right = digitalRead(PIN_IR_RIGHT);
   //black=1 white=0
   Serial.printf("%d %d", left, right);

   if (left == 0 && right == 0) {
      motorWrite(50, 50);
      return 0;
   } else if (left == 0 && right == 1) {
      motorWrite(5, 0);
      return 0;
   } else if (left == 1 && right == 0) {
      motorWrite(0, 5);
      return 0;
   } else {
      motorWrite(0, 0);
      return 1;
   }
}

void getSensorReadings(int sensors[]) {
   int right = 0;
   int left = 0;
   int count = 0;
   while (count < 10) {
      right += digitalRead(4);
      left += digitalRead(15);
      delay(8);
      count++;
   }
   if (right > 7)
   right = 1;
   else
   right = 0;

   if (left > 7)
   left = 1;
   else
   left = 0;

   sensors[0] = left;
   sensors[1] = right;
}

void followPath(String path) {
   char m;
   for (int i = 0; i < path.length(); ++i)
   {
      m = path.charAt(i);
      if (m == 'F') {
         encoderForward(48);
      }
      else if (m == 'R') {
         //encoderTurnRight();
      }
      else if (m == 'L') {
         //encoderTurnLeft();
      }
      else {
         //encoderStop();
      }
   }
}

void followPath1(String path) {
   int sensors[2];
   char m;
   Serial.println("Starts");
   for (int i = 0; i < path.length(); i++) {
      Serial.printf("At a junction\ni: %d \n",  i);

      m = path.charAt(i);
      Serial.println(m);

      if (m == 'F') {
         Serial.println("Move Forward");
         //motorWrite(4, 2);
         getSensorReadings(sensors);
         while (!(sensors[1] && sensors[0])) {

            // 0:Left 1:Right
            Serial.printf("%d %d\n", sensors[1], sensors[0]);
            if (sensors[1] && !sensors[0]) {
               Serial.println("Adjust to Right");
               motorStop();
               delay(500);
               while (sensors[1] || sensors[0]) {
                  sensors[1] = digitalRead(4);
                  sensors[0] = digitalRead(15);
                  if (sensors[1] && sensors[0]) {
                     break;
                  }
                  if (sensors[1] && sensors[0]) {
                     motorWrite(0, 0);

                     delay(500);
                     motorWrite(4, 2);
                     delay(500);
                     break;
                  }
                  motorWrite(-3, 2);
                  delay(50);
               }
               motorStop();
               delay(1000);
            }
            if (!sensors[1] && sensors[0]) {
               Serial.println("Adjust to Left");
               motorStop();
               delay(500);
               while (sensors[1] || sensors[0]) {
                  sensors[1] = digitalRead(4);
                  sensors[0] = digitalRead(15);
                  if (sensors[1] && sensors[0]) {
                     break;
                  }
                  if (sensors[1] && sensors[0]) {
                     motorWrite(0, 0);
                     delay(500);
                     motorWrite(4, 2);
                     delay(500);
                     break;
                  }
                  motorWrite(3, -2);
                  delay(50);
               }
               motorStop();
               delay(1000);
            }
            motorWrite(4, 2);
            getSensorReadings(sensors);

         }
         Serial.println("Stop Moving Forward");
         motorWrite(0, 0);

      } else if (m == 'R') {
         Serial.println("Turn Right");

         pixelColorWave(50, 0, 0); pixelOff(); // red ring
         motorWrite(0, 0);
         delay(500);

         turn(85);



      } else if (m == 'L') {
         Serial.println("Turn Left");

         pixelColorWave(50, 0, 0); pixelOff(); // red ring
         motorWrite(0, 0);
         delay(500);

         turn(-70);

      } else if (m == 'B') {
         Serial.println("Destiniation");
         motorStop();
         delay(500);

      } else {
         // Unsuported instruction
      }
      getSensorReadings(sensors);
      motorWrite(4, 2);
      while (sensors[0] && sensors[1]) {
         delay(10);
         getSensorReadings(sensors);
      }
      delay(300);
      motorStop();
      getSensorReadings(sensors);
      if (sensors[1] && !sensors[0]) {
         Serial.println("Adjust to Right");
         motorStop();
         delay(500);
         while (sensors[1] || sensors[0]) {
            sensors[1] = digitalRead(4);
            sensors[0] = digitalRead(15);
            if (sensors[1] && sensors[0]) {
               break;
            }
            if (sensors[1] && sensors[0]) {
               motorWrite(0, 0);

               delay(500);
               motorWrite(4, 2);
               delay(500);
               break;
            }
            motorWrite(-3, 2);
            delay(50);
         }
         motorStop();
         delay(1000);
      }
      if (!sensors[1] && sensors[0]) {
         Serial.println("Adjust to Left");
         motorStop();
         delay(500);
         while (sensors[1] || sensors[0]) {
            sensors[1] = digitalRead(4);
            sensors[0] = digitalRead(15);
            if (sensors[1] && sensors[0]) {
               break;
            }
            if (sensors[1] && sensors[0]) {
               motorWrite(0, 0);
               delay(500);
               motorWrite(4, 2);
               delay(500);
               break;
            }
            motorWrite(3, -2);
            delay(50);
         }
         motorStop();
         delay(1000);
      }

   }
   motorStop();

   pixelColorWave(0, 0, 50); pixelOff(); // blue ring
   Serial.println("Arrived at destination");
}
*/
