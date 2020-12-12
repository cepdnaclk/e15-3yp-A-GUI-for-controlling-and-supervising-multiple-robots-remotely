
#ifdef ENABLE_PORTS

void portsBegin() {

  expander.begin(EXPANDER_ADDRESS);
  expander.pinMode(PIN_EXPANDER_RED, OUTPUT);
  expander.pinMode(PIN_EXPANDER_GREEN, OUTPUT);
  expander.pinMode(PIN_EXPANDER_BLUE, OUTPUT);

}

/*
void redLED(byte n) {
  expander.digitalWrite(PIN_EXPANDER_RED, (n == 1) ? HIGH : LOW);
}

void greenLED(byte n) {
  expander.digitalWrite(PIN_EXPANDER_GREEN, (n == 1) ? HIGH : LOW);
}

void blueLED(byte n) {
  expander.digitalWrite(PIN_EXPANDER_BLUE, (n == 1) ? HIGH : LOW);
}
*/

#else
// If PORTS aren't enabled, we are defining null functions to avoid syntax errors in the code

void portsBegin() {
  Serial.println(">> info:Ports (not enabled)");
}
/*
void redLED(byte n) {}
void greenLED(byte n) {}
void blueLED(byte n) {}
*/
#endif
