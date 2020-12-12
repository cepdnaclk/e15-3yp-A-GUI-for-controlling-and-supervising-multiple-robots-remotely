/*
  Constructor Function
  --- motorSetup();

  General Functions
  --- motorWrite(left, right);
  --- motorStop();

  Implemented Functions
  --- reverse(distance);
  --- forward(distance);
  --- turnLeft(angle);
  --- turnRight(angle);
*/


#ifdef ENABLE_COMPASS
void turn(float angle) {
  if (abs(angle) > 180) {
    // Invalid angle
  } else {
    if (angle > 0 ) turnRight(angle);
    if (angle < 0 ) turnLeft((-1)*angle);
    else motorStop();
  }
}
void turnRight() {
  float targetAngle = 0;
  if (head == 0) {
    targetAngle = 268;
    head = 1;
  }
  else if (head == 1) {
    targetAngle = 7;
    head = 2;
  }
  else if (head == 2) {
    targetAngle = 96;
    head = 3;
  }
  else if (head == 3) {
    targetAngle = 184;
    head = 0;
  }
  Serial.printf("Target Angle: %f\n", targetAngle);
  motorWrite(3, -2);
  float currentAngle = getAngle();
  while (abs(currentAngle - targetAngle) > 5) {
    currentAngle = getAngle();
    Serial.printf("Current Angle: %f\n", currentAngle);
    delay(1);
  }
  motorWrite(0, 0);
}
void turnLeft() {
  float targetAngle = 0;
  if (head == 0) {
    targetAngle = 96;
    head = 3;
  }
  else if (head == 1) {
    targetAngle = 184;
    head = 0;
  }
  else if (head == 2) {
    targetAngle = 268;
    head = 1;
  }
  else if (head == 3) {
    targetAngle = 7;
    head = 2;
  }
  Serial.printf("Target Angle: %f\n", targetAngle);
  motorWrite(-5, 5);
  float currentAngle = getAngle();
  while (abs(currentAngle - targetAngle) > 5) {
    currentAngle = getAngle();
    Serial.printf("Current Angle: %f\n", currentAngle);
    delay(1);
  }
  motorWrite(0, 0);
}
void turnLeft(float angle) {  // angle in degrees
  angle -= 5;
  float currentAngle = getAngle();
  float targetAngle = currentAngle - angle;
  Serial.print("Target Angle: ");
  Serial.println(targetAngle);
  Serial.print("Current Angle: ");
  Serial.println(currentAngle);
  motorWrite(-3, 2);
  Serial.println("Turning Left...........");
  while (currentAngle > targetAngle) {
    delay(10);
    currentAngle = getAngle();
    if (targetAngle < 0 && currentAngle > 180) {
      currentAngle -= 360;
    }
    Serial.println(currentAngle);
  }
  motorStop();
  Serial.println("Finished............");
}
void turnRight(float angle) {   // angle in degrees
  angle -= 5;
  float currentAngle = getAngle();
  float targetAngle = currentAngle + angle;
  Serial.print("Target Angle: ");
  Serial.println(targetAngle);
  Serial.print("Current Angle: ");
  Serial.println(currentAngle);
  motorWrite(3, -2);
  Serial.println("Turning Right...........");
  while (currentAngle < targetAngle) {
    delay(10);
    currentAngle = getAngle();
    if (targetAngle > 360 && currentAngle < 180) {
      currentAngle += 360;
    }
    Serial.println(currentAngle);
  }
  motorStop();
  Serial.println("Finished............");
}
#else
void turn(float angle) {
  motorStop();
  Serial.println(">> error: compass not available");
}
void turnLeft(float angle) {  // angle in degrees
  motorStop();
  Serial.println(">> error: compass not available");
}
void turnRight(float angle) {   // angle in degrees
  motorStop();
  Serial.println(">> error: compass not available");


}
#endif


#if defined(ENABLE_SERVO)

void motorSetup() {
  motorAttach();
  //motorWrite(0, 0);
}

void motorAttach() {
  servoRight.attach(PIN_SERVO_RIGHT, SERVO_MIN_US, SERVO_MAX_US);
  servoLeft.attach(PIN_SERVO_LEFT, SERVO_MIN_US, SERVO_MAX_US);
}
void motorDetach() {
  servoRight.detach();
  servoLeft.detach();
  motorState = false;
}
void motorWrite(int8_t left, int8_t right) {
  // 90 93
  servoRight.write(90 - right);
  servoLeft.write(93 + left);
}

void forward(float distance) {  // distance in cm
  motorWrite(50, 50);
  float delayForDistance = (distance / VELOCITY) * 1000;
  delay(delayForDistance);
}
void reverse(float distance) {  // distance in cm
  motorWrite(-50, -50);
  float delayForDistance = (distance / VELOCITY) * 1000;
  delay(delayForDistance);
}
void motorStop() {
  motorWrite(0, 0);
  //motorDetach();
}
void motorTest() {}

#elif defined(ENABLE_PWM)

void motorSetup() {

  Serial.println(">> MotorControl : Begin...");

  pinMode(PIN_MOT_A, OUTPUT);
  pinMode(PIN_MOT_B, OUTPUT);

  ledcSetup(LEDC_CHANNEL_A, LEDC_BASE_FREQ, LEDC_RESOLUTION_BITS);
  ledcSetup(LEDC_CHANNEL_B, LEDC_BASE_FREQ, LEDC_RESOLUTION_BITS);

  ledcAttachPin(PIN_PWM_A, LEDC_CHANNEL_A);
  ledcAttachPin(PIN_PWM_B, LEDC_CHANNEL_B);

  ledcWrite(LEDC_CHANNEL_A, 0);
  ledcWrite(LEDC_CHANNEL_B, 0);

  motorWrite(0, 0);

}
void motorWrite(int leftSpeed, int rightSpeed) {

  if (leftSpeed > 30)leftSpeed -= 10;
  if (rightSpeed > 30)rightSpeed += 12; // drift

  // ------ map  the speed with correct & acceptable range --------------------
  leftSpeed = constrain(leftSpeed, -1 * MAX_MOTOR_SPEED , MAX_MOTOR_SPEED);
  rightSpeed = constrain(rightSpeed , -1 * MAX_MOTOR_SPEED , MAX_MOTOR_SPEED);

  // ------ motor rotating directions -----------------------------------------
  leftMotorDir = (leftSpeed >= 0) ? 1 : 0;
  rightMotorDir = (rightSpeed >= 0) ? 1 : 0;

  //------- check motor directions --------------------------------------------
  if (leftMotorDir !=  leftMotorDirOld) {
    // Direction changed
    digitalWrite(PIN_MOT_A, (leftMotorDir) ? HIGH : LOW);
    leftMotorDirOld = leftMotorDir;
  }
  if (rightMotorDir !=  rightMotorDirOld) {
    // Direction changed
    digitalWrite(PIN_MOT_B, (rightMotorDir) ? LOW : HIGH);
    rightMotorDirOld = rightMotorDir;
  }
  //---------------------------------------------------------------------------

  rightMotorSpeed = abs(rightSpeed);
  leftMotorSpeed = abs(leftSpeed);

  // Analog write function for ESP32
  ledcWrite(LEDC_CHANNEL_A, leftMotorSpeed);
  ledcWrite(LEDC_CHANNEL_B, rightMotorSpeed);

}
void reverse(float distance) {
  motorWrite(-100, -100);
}
void forward(float distance) {
  motorWrite(100, 100);
}
void motorStop() {
  motorWrite(0, 0);
}

void motorTest() {
  for (int i = 0; i < 255; i++) {
    motorWrite(i, i);
    delay(100);
  }
  motorWrite(0, 0);
  delay(500);

  for (int i = 255; i > 0; i--) {
    motorWrite(i, i);
    delay(100);
  }

  motorWrite(0, 0);
  delay(500);
}

void ledcAnalogWrite(uint8_t channel, uint32_t value) { // , uint32_t valueMax = 255
  uint32_t duty = 16 * value; //(4095 / valueMax) * min(value, valueMax);
  ledcWrite(channel, duty);
}

#else

void motorSetup() {
  Serial.println(">> info:Motors (not enabled)");
}
void motorWrite(int left, int right) {}
void reverse(float distance) {}
void forward(float distance) {}
void motorStop() {}

void motorTest() {}

#endif
