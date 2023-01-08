#include <Wire.h>
#include <VL53L1X.h>




const int cmd_TOGGLESTREAM = 's';
const int cmd_HANDSHAKE = 'h';
const int cmd_DEBUG = 'd';
const int cmd_NORMAL = 'n';
const int cmd_PING = 'p';
const int cmd_GET_RANGE = 't';
const int cmd_GET_RANGE_FAST='f';
const int cmd_FAST_READ_ON='x';
const int cmd_FAST_READ_OFF='z';
const int cmd_RESET_TIME = 'r';


bool debug = false;
bool stream = false;
int range=0;
char command;

unsigned long start_time = 0;


VL53L1X sensor;


void setup(void) {
 
   while (!Serial) {}
    Serial.begin(115200);
    Wire.begin();
    Wire.setClock(400000); // use 400 kHz I2C
    sensor.setTimeout(500);
    if (!sensor.init()){
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
    }
    sensor.setDistanceMode(VL53L1X::Short);
    sensor.setMeasurementTimingBudget(20000);
    
}

void loop(void) {

  sensor.read();



  if (Serial.available()) {

    command = Serial.read();
    switch (command) {

      case cmd_TOGGLESTREAM:
        if (!stream){
          sensor.startContinuous(50);
        }else{
          sensor.stopContinuous();
        }
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
      case cmd_FAST_READ_ON:
        sensor.startContinuous(20);
        break;
      case cmd_FAST_READ_OFF:
        sensor.stopContinuous();
        break;
      case cmd_GET_RANGE:
        range=sensor.readSingle(true);
        Serial.print(millis() - start_time);
        Serial.print(",");
        Serial.println(range);
        break;
      case cmd_GET_RANGE_FAST:
        range=sensor.read(true);
        Serial.print(millis() - start_time);
        Serial.print(",");
        Serial.println(range);
        break;
    }
  }

  if (stream) {
    Serial.println(sensor.ranging_data.range_mm);
  }
  if (debug) {
    Serial.print("tempo (ms): ");
    Serial.print(millis() - start_time);
    Serial.print("\t");
    Serial.print("temperatura (C): ");
    Serial.println(sensor.ranging_data.range_mm);
  }
}



void handshake() {
  Serial.println('R');
}