/*
  one pulse widths 26uS, cycle 52uS
  26uS = 200ns * 65 ticks
*/

#ifdef ENABLE_INFARED

void beginInfared() {

  //-- setup receivers --------------------------------------------------------

  for (int i = 0; i < IR_SENSOR_COUNT; i++) {
    if ((rmt_rx[i] = rmtInit(pin_rmtRx[i], false, RMT_MEM_64)) == NULL)
      Serial.println(">> error: Infared (Rx module failed)\n");
    realTickRx = rmtSetTick(rmt_rx[i], 5000); // x10 uS
  }

  //-- setup transmiter -------------------------------------------------------
  if ((rmt_send = rmtInit(PIN_IR_TX, true, RMT_MEM_64)) == NULL)
    Serial.println(">> error: Infared (Tx module failed)\n");

  // Configure tick widths of the transmiter
  realTickTx = rmtSetTick(rmt_send, 200);

  Serial.printf(">> Infared: real tick set to: Tx: %fns | Rx: %fns\n\n", realTickTx, realTickRx);

  //-- Configure interrupt function for receiver ------------------------------

  rmtRead(rmt_rx[0], irReceive_0);
  rmtRead(rmt_rx[1], irReceive_1);
  rmtRead(rmt_rx[2], irReceive_2);
  rmtRead(rmt_rx[3], irReceive_3);

#if IR_SENSOR_COUNT==6
  rmtRead(rmt_rx[4], irReceive_4);
  rmtRead(rmt_rx[5], irReceive_5);
#endif

  //-- Generate carrier waveform of 38KHz -------------------------------------
  for (int i = 0; i < 256; i++) {
    dataTx[i].val = 0x80010000 + ((32) << 17) + ((32) << 1);
  }
}

// -- Send Signals -------------------------------------------------------------

void sendWaveform(unsigned int value) {
  sendWaveform(value, IR_BIT_LEN); // default: 32bits
}
void sendWaveform(unsigned int value, int len) {
  uint8_t bitI;
  if (IR_DEBUG) Serial.print("Send \t> "); // LSB First

  // Store the last value send through IR LEDs
  rmtValueSend = value;

  // Sends a starting bit of 10 ticks
  sendBit(10, 10);

  // send signal bits
  for (uint8_t i = 0; i < len; i++) {
    bitI = bitRead(value, i);

    if (bitI == 1) sendBit(PULSE_HIGH_TICKS, PULSE_LOW_TICKS);
    else sendBit(PULSE_LOW_TICKS, PULSE_HIGH_TICKS);
    if (IR_DEBUG) Serial.print(bitI);
  }
  // Sends a ending bit of 10 ticks
  sendBit(10, 10);
  if (IR_DEBUG) Serial.printf(" (%d)\n", value);

}
void sendBit(short pulseHigh, short pulseLow) {
  rmtWrite(rmt_send, dataTx, pulseHigh);
  delayMicroseconds(26 * (pulseHigh + pulseLow));
}

// -- Receive Signals ----------------------------------------------------------

void parseRmtData(rmt_data_t* items, size_t len, uint8_t rId) {
  if (len == IR_BIT_LEN + 2 ) {
    //if (len < 100 ) {
    rmt_data_t* it = &items[0];
    unsigned int value = 0;

    if (IR_DEBUG) Serial.print("Receive\t< "); // LSB First

    // trim the first and last signal bits
    for (size_t i = 1; i < (len - 1); i++) {
      it = &items[i];

      if ((it->duration0 < 200) && (it->duration1 < 200) && (it->duration0 > 20) && (it->duration1 > 20)) {
        //if (IR_DEBUG) Serial.printf("%d", (it->duration0 > it->duration1));
        bitWrite(value, i - 1, (it->duration0 > it->duration1));

      } else {
        if (IR_DEBUG) Serial.printf("(%d,%d)", it->duration0, it->duration1);
      }
    }

    Serial.printf("      << %d (%d)\n", value, rId);
    //instructionExecution(value);

  }
}

// Hamming Code related functions ----------------------------------------------

unsigned int encodedValue(unsigned int value)  {
  // REM: len isn't using in here

  char bitArray[32];
  int count = 0;

  for (int i = 1; i <= 32; ++i) {
    if (ceil(log2(i)) != floor(log2(i)))  {
      bitArray[32 - i] = bitRead(value, count++) + '0';
    } else {
      bitArray[32 - i] = '0';
    }
  }

  // Debugging -------
  for (int i = 0; i < 32; ++i) {
    Serial.print(bitArray[i]);
  }
  Serial.println();
  // -----------------

  for (int i = 0; i < 6; ++i)  {
    int evenCount = 0;
    for (int j = 1; j <= 32; ++j)  {
      if (ceil(log2(j)) != floor(log2(j)) && bitRead(j, i) == 1)  {
        if (bitArray[32 - j] == '1')
          ++evenCount;
      }
    }
    if (evenCount % 2 == 0)bitArray[32 - ((int)(pow(2, i)))] = '0';
    else bitArray[32 - ((int)(pow(2, i)))] = '1';
  }

  // Debugging -------
  for (int i = 0; i < 32; ++i) {
    Serial.print(bitArray[i]);
  }
  Serial.println();
  // -----------------
  return charToInt(bitArray, 32);

}
unsigned int decodedValue(unsigned int value)  {
  // REM: len isn't using in here

  char checkBitArray[6];
  int count = 5;

  for (int i = 0; i < 6; ++i)  {
    Serial.printf("%d : ", i);

    int evenCount = 0;

    for (int j = 0; j < 32; ++j)  {
      if (bitRead((j + 1), i) == 1)  {
        Serial.printf("%d (%d), ", j, bitRead(value, j));
        if (bitRead(value, j) == 1)
          ++evenCount;
      }
    }
    Serial.println();
    if (evenCount % 2 == 0) checkBitArray[count--] = '0';
    else checkBitArray[count--] = '1';
  }

  Serial.println("-----------Check Bits----------");
  for (int i = 0; i < 6; ++i) {
    Serial.print(checkBitArray[i]);
  }
  Serial.println();

  unsigned int checkValue = charToInt(checkBitArray, 6);
  Serial.println(checkValue);

  if (checkValue == 0)  {
    return value;

  } else  {
    char bitArray[32];
    int count = 0;
    for (int i = 1; i <= 32; ++i)  {
      bitArray[32 - i] = bitRead(value, count++) + '0';
    }
    for (int i = 0; i < 32; ++i) {
      Serial.print(bitArray[i]);
    }
    Serial.println();
    bitArray[32 - checkValue] = !(bitArray[32 - checkValue] - '0') + '0';
    for (int i = 0; i < 32; ++i) {
      Serial.print(bitArray[i]);
    }
    Serial.println();
    return charToInt(bitArray, 32);
  }
}
unsigned int charToInt(char* bitArray, int len) {
  // Usage: charToInt("1001010111100100", 16)
  unsigned int value = 0;
  for (int i = 0; i < len; i++) {
    value = (value << 1) | (bitArray[i] == '1');
  }
  return value;
}

void sendTestSignal() {
  unsigned int randNumber = esp_random();
  sendWaveform(randNumber, IR_BIT_LEN);
}

#else

void beginInfared() {
  Serial.println(">> info: Infared (not enabled)");
}
void sendTestSignal() {}

void sendWaveform(unsigned int value) {}
void sendWaveform(unsigned int value, int len) {}
void sendBit(short bitI, short pulseHigh, short pulseLow) {}
void parseRmtData(int* items, int len,  uint8_t rId) {}

unsigned int encodedValue(unsigned int value) {
  return 0;
}
unsigned int decodedValue(unsigned int value) {
  return 0;
}
unsigned int charToInt(char* bitArray, int len) {
  return 0;
}

#endif
