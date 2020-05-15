/*
   RadioLib SX128x Transmit with Interrupts Example

   This example transmits LoRa packets with one second delays
   between them. Each packet contains up to 256 bytes
   of data, in the form of:
    - Arduino String
    - null-terminated char array (C-string)
    - arbitrary binary data (byte array)

   Other modules from SX128x family can also be used.

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// SX1280 has the following connections:
// NSS pin:   10
// DIO1 pin:  2
// NRST pin:  3
// BUSY pin:  9
SX1280 lora = new Module(10, 2, 3, 9);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1280 lora = RadioShield.ModuleA;

// save transmission state between loops
int transmissionState = ERR_NONE;

void setup() {
  Serial.begin(9600);

  // initialize SX1280 with default settings
  Serial.print(F("[SX1280] Initializing ... "));
  // carrier frequency:           2400.0 MHz
  // bandwidth:                   812.5 kHz
  // spreading factor:            9
  // coding rate:                 7
  // output power:                10 dBm
  // preamble length:             12 symbols
  // CRC:                         enabled
  int state = lora.begin();
  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // set the function that will be called
  // when packet transmission is finished
  lora.setDio1Action(setFlag);

  // start transmitting the first packet
  Serial.print(F("[SX1280] Sending first packet ... "));

  // you can transmit C-string or Arduino string up to
  // 256 characters long
  transmissionState = lora.startTransmit("Hello World!");

  // you can also transmit byte array up to 256 bytes long
  /*
    byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                      0x89, 0xAB, 0xCD, 0xEF};
    state = lora.startTransmit(byteArr, 8);
  */
}

// flag to indicate that a packet was sent
volatile bool transmittedFlag = false;

// disable interrupt when it's not needed
volatile bool enableInterrupt = true;

// this function is called when a complete packet
// is transmitted by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void setFlag(void) {
  // check if the interrupt is enabled
  if(!enableInterrupt) {
    return;
  }

  // we sent a packet, set the flag
  transmittedFlag = true;
}

void loop() {
  // check if the previous transmission finished
  if(transmittedFlag) {
    // disable the interrupt service routine while
    // processing the data
    enableInterrupt = false;

    // reset flag
    transmittedFlag = false;

    if (transmissionState == ERR_NONE) {
      // packet was successfully sent
      Serial.println(F("transmission finished!"));

    } else {
      Serial.print(F("failed, code "));
      Serial.println(transmissionState);

    }

    // wait a second before transmitting again
    delay(1000);

    // send another one
    Serial.print(F("[SX1280] Sending another packet ... "));

    // you can transmit C-string or Arduino string up to
    // 256 characters long
    transmissionState = lora.startTransmit("Hello World!");

    // you can also transmit byte array up to 256 bytes long
    /*
      byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                        0x89, 0xAB, 0xCD, 0xEF};
      int state = lora.startTransmit(byteArr, 8);
    */

    // we're ready to send more packets,
    // enable interrupt service routine
    enableInterrupt = true;
  }
}
