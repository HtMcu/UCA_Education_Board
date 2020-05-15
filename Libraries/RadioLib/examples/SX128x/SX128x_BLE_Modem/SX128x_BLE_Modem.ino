/*
   RadioLib SX128x BLE Modem Example

   This example shows how to use BLE modem in SX128x chips.
   RadioLib does not provide BLE protocol support (yet),
   only compatibility with the physical layer.

   NOTE: The sketch below is just a guide on how to use
         BLE modem, so this code should not be run directly!
         Instead, modify the other examples to use BLE
         modem and use the appropriate configuration
         methods.

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
SX1280 ble = new Module(10, 2, 3, 9);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1280 ble = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  // initialize SX1280 with default settings
  Serial.print(F("[SX1280] Initializing ... "));
  // carrier frequency:           2400.0 MHz
  // bit rate:                    800 kbps
  // frequency deviation:         400.0 kHz
  // output power:                10 dBm
  // preamble length:             16 bits
  // data shaping:                Gaussian, BT = 0.5
  // CRC:                         enabled, CRC16 (CCIT)
  int state = ble.beginBLE();
  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // if needed, you can switch between LoRa and FSK modes
  //
  // ble.begin()       start LoRa mode (and disable BLE)
  // lora.beginBLE()   start BLE mode (and disable LoRa)

  // the following settings can also
  // be modified at run-time
  state = ble.setFrequency(2410.5);
  state = ble.setBitRate(250);
  state = ble.setFrequencyDeviation(100.0);
  state = ble.setOutputPower(5);
  state = ble.setDataShaping(1.0);
  state = ble.setAccessAddress(0x12345678);
  if (state != ERR_NONE) {
    Serial.print(F("Unable to set configuration, code "));
    Serial.println(state);
    while (true);
  }

  #warning "This sketch is just an API guide! Read the note at line 6."
}

void loop() {
  // BLE modem can use the same transmit/receive methods
  // as the LoRa modem, even their interrupt-driven versions

  // transmit BLE packet
  int state = ble.transmit("Hello World!");
  /*
    byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                      0x89, 0xAB, 0xCD, 0xEF};
    int state = ble.transmit(byteArr, 8);
  */
  if (state == ERR_NONE) {
    Serial.println(F("[SX1280] Packet transmitted successfully!"));
  } else if (state == ERR_PACKET_TOO_LONG) {
    Serial.println(F("[SX1280] Packet too long!"));
  } else if (state == ERR_TX_TIMEOUT) {
    Serial.println(F("[SX1280] Timed out while transmitting!"));
  } else {
    Serial.print(F("[SX1280] Failed to transmit packet, code "));
    Serial.println(state);
  }

  // receive BLE packet
  String str;
  state = ble.receive(str);
  /*
    byte byteArr[8];
    int state = ble.receive(byteArr, 8);
  */
  if (state == ERR_NONE) {
    Serial.println(F("[SX1280] Received packet!"));
    Serial.print(F("[SX1280] Data:\t"));
    Serial.println(str);
  } else if (state == ERR_RX_TIMEOUT) {
    Serial.println(F("[SX1280] Timed out while waiting for packet!"));
  } else {
    Serial.print(F("[SX1280] Failed to receive packet, code "));
    Serial.println(state);
  }
}
