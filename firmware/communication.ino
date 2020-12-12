
/*
  int messageEncode(source, destination, opCode, data)
   This function encode data into to message int

  message_t messageDecode(uint32_t inst)
   This function will decode a message to rmtMessage struct
*/

uint32_t instructionBuffer[3]={0};
uint32_t communicationId = 0;
uint32_t message;

// OPCODES -----------------------------------------------------------------
#define OPCODE_HANDSHAKE 0b0000
#define OPCODE_SEND_COLOR 0b0001

// Node broadcastId is the ID that address all the robots
#define NODE_BROADCAST_ID 0

#define DATA_HANDSHAKE_REQ 0
#define DATA_HANDSHAKE_ACK 1
#define DATA_HANDSHAKE_ACK2 2
#define DATA_HANDSHAKE_REJECT 3

//Check for synchronization
void instructionExecution(uint32_t inst) {
  Serial.println("Starting Instruction Execution");

  rmtMessage_t current = messageDecode(inst);

  if (current.sourceId == ROBOT_ID) {
    // This message was sent by the robot itself. So ignore this
    Serial.println("ignored");
  }

  else if (current.destinationId == ROBOT_ID || current.destinationId == NODE_BROADCAST_ID) {
    // This is a broadcast message on the network. Better to listen
    Serial.println("Executing");
    if (current.opCode ==  OPCODE_HANDSHAKE && communicationId == 0 ) {  // Handshake Message
      Serial.println("Handshake Begins");
      protocolHandshake_Execution(current);
    }
    else if (current.opCode == 1 && communicationId == current.sourceId) {
      Serial.println("Color");
      pixelColorWave(50, 0, 50);
    }

  }
  else if (current.destinationId == ROBOT_ID) {
    // This message is directly targeted to me. Let's process

    if (current.opCode ==  OPCODE_HANDSHAKE) {   // Handshake Message
      Serial.println("Handshake Continue");
      protocolHandshake_Execution(current);
    }


  }
}

void protocolHandshake_Execution(struct rmtMessage_t current) {
  int waveform = 0;

  if (current.data == DATA_HANDSHAKE_REQ) {
    // accepts the broadcast request
    Serial.printf("<< Handshake request received from %d. Let's reply ACK.\n", current.sourceId);

    waveform = messageEncode(ROBOT_ID, current.sourceId, OPCODE_HANDSHAKE, DATA_HANDSHAKE_ACK);
    Serial.print("Waveform: ");
    Serial.println(waveform);
    sendWaveform(waveform, 32);

  } else if (current.data == DATA_HANDSHAKE_ACK) {
    // Broadcast request was accepted by current.sourceId. We send it the ACK2
    waveform = messageEncode(ROBOT_ID, current.sourceId, OPCODE_HANDSHAKE, DATA_HANDSHAKE_ACK2);
    //waveform = messageEncode(5, 5, 5, 5);
    Serial.printf(">> Sending ACK2 back to %d \n", current.sourceId);
    delayMicroseconds(80000);
    delayMicroseconds(80000);
    delayMicroseconds(80000);
    sendWaveform(waveform, 32);
    communicationId = current.sourceId;

   /* waveform = messageEncode(ROBOT_ID, current.sourceId, OPCODE_SEND_COLOR, 7);
    delayMicroseconds(80000);
    delayMicroseconds(80000);
    delayMicroseconds(80000);
    sendWaveform(waveform, 32);*/


    // delay(3000);
    //waveform = messageEncode(ROBOT_ID, communicationId, 1, 1);


  } else if (current.data == DATA_HANDSHAKE_ACK2) {
    Serial.printf("<< Received ACK2 message from %d \n", current.sourceId);
    /*waveform = messageEncode(ROBOT_ID, current.sourceId, OPCODE_SEND_COLOR, 7);
    delayMicroseconds(80000);
    delayMicroseconds(80000);
    delayMicroseconds(80000);
    sendWaveform(waveform, 32);*/
    communicationId = current.sourceId;

  } else if (current.data == DATA_HANDSHAKE_REJECT) {
    communicationId = 0;
  }
}

/*void handleACK() {

  // Wait until receiving new value
  Serial.printf("flagRmtRecieve=%d, value=%d\n", flagRmtRecieve, rmtValueReceived); delay(100);

  while ((flagRmtRecieve == 0) || (rmtValueReceived == 0)) { //  && (rmtValueReceived == rmtValueSend)
    Serial.print("."); delay(100);
    // TODO: Here a troble exists
  }
  Serial.printf("flagRmtRecieve=%d, value=%d\n", flagRmtRecieve, rmtValueReceived); delay(100);


  rmtMessage_t current = messageDecode(rmtValueReceived);

  if ((current.destinationId == ROBOT_ID) && (current.opCode == OPCODE_HANDSHAKE) && (current.data == DATA_HANDSHAKE_ACK 1)) {
    // received a reply from one robot, currently handle this as stateless reply
    Serial.printf("<< Received a reply from %d, Sends ACK2 back\n", current.sourceId);
    sendWaveform(messageEncode(ROBOT_ID, current.sourceId , OPCODE_HANDSHAKE , DATA_HANDSHAKE_ACK2));
  } else {
    messagePrint(current);
    Serial.println("ACK timeout");
  }

  }*/

struct rmtMessage_t messageDecode(uint32_t inst) {
  rmtMessage msg;

  // Message format :
  //    27:22  |  21:17 |  16:13 |  12:0
  // [sourceId] [destId] [opCode] [data]

  msg.instruction = inst;
  msg.sourceId = (inst & 0b00000111110000000000000000000000) >> 22;
  msg.destinationId = (inst & 0b00000000001111100000000000000000) >> 17;
  msg.opCode = (inst & 0b00000000000000011110000000000000) >> 13;
  msg.data = inst & 0b00000000000000000001111111111111;

  //Serial.printf("Message: %d <- %d, opcode:%d, data:%d \n", msg.sourceId, msg.destinationId, msg.opCode, msg.data);
  return msg;
}

// Format: messageEncode(source, destination , opCode , data)
int messageEncode(int source, int destination , int opCode , int data) {
  return (source << 22 | destination << 17 | opCode << 13 | data);
}

void messagePrint(struct rmtMessage_t msg) {
  Serial.printf("Message: %d <- %d, opcode:%d, data:%d (%d)\n", msg.sourceId, msg.destinationId, msg.opCode, msg.data, msg.instruction);
}
