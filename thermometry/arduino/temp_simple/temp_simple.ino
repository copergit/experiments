#include "DallasTemperature.h"
#include "OneWire.h"



bool mode=true;
char command;

#define ONE_WIRE_BUS 2 

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

void setup(void){ 
  Serial.begin(9600); 
  sensors.begin(); 
} 

void loop(void) {

sensors.requestTemperatures();


if (Serial.available()) {
  
    command=Serial.read();
    switch(command){

     case 'p':
       mode=true;
     break;
     
     case 'm':
      mode=false;
     break;
     
    }
}
 
if(mode){
    Serial.println(sensors.getTempCByIndex(0));
    }
    else{
      Serial.print("tempo (ms): ");
      Serial.print(millis());
      Serial.print("\t");
      Serial.print("temperatura (C): ");
      Serial.println(sensors.getTempCByIndex(0));  
      }
}
