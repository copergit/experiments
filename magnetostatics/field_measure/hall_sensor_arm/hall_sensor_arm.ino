#include<mwc_stepper.h>


#define EN_PIN 4
#define DIR_PIN 5
#define STEP_PIN 6
#define PULSE 6400
#define CLOCKWISE 1
#define COUNTERCLOCKWISE 0
#define RPM 50
#define RPM1 25
#define STOP_ONE 2
#define STOP_TWO 3


const int cmd_TOGGLESTREAM = 's';
const int cmd_HANDSHAKE = 'h';
const int cmd_GO_HOME1 = 'w';
const int cmd_GO_HOME2 = 'x';
const int cmd_DEBUG = 'd';
const int cmd_NORMAL = 'n';
const int cmd_PING = 'p';
const int cmd_RESET_TIME = 'r';
const int cmd_MOVE_FORWARD = 'f';
const int cmd_MOVE_BACKWARD = 'b';
const int cmd_SEND_MOVEMENT_FORWARD = 'm';
const int cmd_SEND_MOVEMENT_BACKWARD= 'q';

const int one_turn=6400;
const int min_step=80;
const float resolution=0.1;

String data;

bool debug = false;
bool stream = false;
volatile bool running = true;

char command;
unsigned long start_time = 0;
unsigned int movement=0;


MWCSTEPPER nema23(EN_PIN, DIR_PIN, STEP_PIN);


void setup() {
  
  Serial.begin(9600);
  nema23.init();
  nema23.active(true);

  pinMode(STOP_ONE, INPUT);
  attachInterrupt(digitalPinToInterrupt(STOP_ONE), homestop, CHANGE);
  attachInterrupt(digitalPinToInterrupt(STOP_TWO), homestop, CHANGE);
  Serial.print("Stop status ");
  Serial.println(digitalRead(STOP_ONE));
  Serial.println(digitalRead(STOP_TWO));
  running=digitalRead(STOP_ONE)||digitalRead(STOP_TWO);

}

void loop() {

if (Serial.available()) {

    command = Serial.read();
    switch (command) {

      case cmd_GO_HOME1:
        
        running=digitalRead(STOP_ONE);
        Serial.println("Going home, motor side.");
        if (!running) break;
        nema23.active(false);
        nema23.set(COUNTERCLOCKWISE, RPM1, PULSE);

        while (running){
            nema23.run();
        }
        running=true;

        nema23.set(CLOCKWISE, RPM, PULSE);
        delayMicroseconds(2000);

        for (int i = 0; i < 8*min_step; i++){
              
                nema23.run();
              
	         }
        nema23.active(true);
        break;    

      case cmd_GO_HOME2:
        
        running=digitalRead(STOP_TWO);
        Serial.println("Going home, other side.");
        if (!running) break;
        nema23.active(false);
        nema23.set(CLOCKWISE, RPM, PULSE);

        while (running){
            nema23.run();
        }
        running=true;

        nema23.set(COUNTERCLOCKWISE, RPM1, PULSE);
        delayMicroseconds(2000);

        for (int i = 0; i < 8*min_step; i++){
              
                nema23.run();
              
	         }
        nema23.active(true);
        break;    

      case cmd_MOVE_FORWARD:
        Serial.println("Move forward 1 turn\n");
        nema23.active(false);
        nema23.set(CLOCKWISE, RPM, PULSE);
        for (int i = 0; i < min_step; i++){
            if(running){
		          nema23.run();
            }
	      }
        nema23.active(true);
        break;  
      case cmd_SEND_MOVEMENT_FORWARD:
          data=Serial.readString();
          running=digitalRead(STOP_TWO);
          if (!running) break;
          movement= floor((data.toFloat())/resolution);
          nema23.active(false);
          nema23.set(CLOCKWISE, RPM, PULSE);
          for (int i = 0; i < movement*min_step; i++){
              if(running){
                nema23.run();
              }
	         }
           nema23.active(true);
        break;

      case cmd_SEND_MOVEMENT_BACKWARD:
          data=Serial.readString();
          running=digitalRead(STOP_ONE);
          if (!running) break;
          movement= floor((data.toFloat())/resolution);
          nema23.active(false);
          nema23.set(COUNTERCLOCKWISE, RPM1, PULSE);
          for (int i = 0; i < movement*min_step; i++){
            if(running){
              nema23.run();
            }
	         }
           nema23.active(true);
        break;

       case cmd_MOVE_BACKWARD:
        Serial.println("Move backwards 1 turn\n");
        nema23.set(COUNTERCLOCKWISE, RPM1, PULSE);
        nema23.active(false);
        for (int i = 0; i < one_turn; i++){
          if(running){
		        nema23.run();
          }
	      }
        nema23.active(true);
        break; 

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
      }
  }



}

void homestop() {
 running=false;
 Serial.println("Stop!");
}

void handshake() {
  Serial.println('R');
}