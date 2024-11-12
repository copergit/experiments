
#define BUTTON_PIN 2


void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), blinkLed, RISING);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void blinkLed() {
 Serial.print("Detected\n");
}
