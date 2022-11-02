#include "DallasTemperature.h"
#include "OneWire.h"




const int cmd_TOGGLESTREAM = 's';
const int cmd_HANDSHAKE = 'h';
const int cmd_DEBUG = 'd';
const int cmd_NORMAL = 'n';
const int cmd_PING = 'p';
const int cmd_GET_TEMP = 't';
const int cmd_RESET_TIME = 'r';


bool debug = false;
bool stream = false;
char command;

unsigned long start_time = 0;

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(void) {
  Serial.begin(9600);
  sensors.begin();
}

void loop(void) {

  sensors.requestTemperatures();


  if (Serial.available()) {

    command = Serial.read();
    switch (command) {

      case cmd_TOGGLESTREAM:
        stream = !stream;
        break;

      case cmd_HANDSHAKE:
        handshake();
        break;
      case cmd_DEBUG:
        debug = true;
        break;
      case cmd_NORMAL:
        debug = false;
        stream = false;
        break;
      case cmd_PING:
        Serial.println("PONG");
        break;
      case cmd_RESET_TIME:
        start_time = millis();
        break;
      case cmd_GET_TEMP:
        Serial.print(millis() - start_time);
        Serial.print(",");
        Serial.println(sensors.getTempCByIndex(0));
        break;
    }
  }

  if (stream) {
    Serial.println(sensors.getTempCByIndex(0));
  }
  if (debug) {
    Serial.print("tempo (ms): ");
    Serial.print(millis() - start_time);
    Serial.print("\t");
    Serial.print("temperatura (C): ");
    Serial.println(sensors.getTempCByIndex(0));
  }
}



void handshake() {
  Serial.println('R');
}