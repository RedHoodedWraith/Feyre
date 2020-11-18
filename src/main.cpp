/***
 * Title: Feyre LoRa Bond
 * Author: Rowan Rathod
 * Last Updated: 15 November 2020
 *
 */

#include "feyre_header.h"

String buffer = "";
int packetSize = -1;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    // Initialises External Serial Port
    Serial.begin(DEFAULT_SERIAL_SPEED);

    // Turns on the Status Light if the Serial Port failed to initialise
    if(!Serial){
        digitalWrite(LED_BUILTIN, HIGH);
        while(!Serial);
    }
    // Ensures the Status Light is Off upon success
    digitalWrite(LED_BUILTIN, LOW);


    // Initialises Inter-Component Serial Port
    Serial3.begin(LOCAL_COMPONENT_SERIAL_SPEED);
    // Blinks Status Light if Serial Port failed to initialise
    bool lightOn = true;
    while(!Serial3) {
        digitalWrite(LED_BUILTIN, lightOn ? HIGH : LOW);
        lightOn = !lightOn;
        delay(500);
    }
    // Ensures the Status Light is Off upon success
    digitalWrite(LED_BUILTIN, LOW);

    // Turns off Status LED of Serial Port initialised successfully
    digitalWrite(LED_BUILTIN, LOW);

    // Prints out the app name
    Serial.println(APP_NAME);

    // Attempts to Start the LoRa Module
    Serial.println("Starting LoRa Module...");
    while(!LoRa.begin(DEFAULT_FREQ)) {
        Serial.println("Starting LoRa failed! Trying again in");
        for(int i=5; i>0; i--){
            Serial.print(String(i));
            Serial.println("...");
            delay(1000);
        }
    }
    Serial.println("Success!");
}

void loop() {
    buffer = "";
    packetSize = LoRa.parsePacket();
    if (packetSize) {
        // received a packet
        Serial.print("Received packet '");

        // read packet
        while (LoRa.available()) {
            buffer += (char)LoRa.read();
        }
        Serial.print(buffer);
        Serial.print("'\n");

        // Sends received buffer to the Rhys Module
        Serial3.println(buffer);

        // print RSSI of packet
        Serial3.print("' with RSSI ");
        Serial3.println(LoRa.packetRssi());
    }
}