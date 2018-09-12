/*
 * Motion Light Switch
 * 
 * Outputs HIGH (for a relay which replaces a manual light switch)
 * once a motion sensor has detected a motion at preset light conditions.
 */

const int pinLightSwitch = 13;
const int pinMotionSensor = 2;
const int pinLightSensor = A0;

int stateLightSwitch = LOW;

boolean motionDetected;
uint32_t lastMotionTime;


/*
 * Light intensity ranges:
 *    < 10  = dark
 *    < 200 = dim
 *    < 500 = light
 *    < 800 = bright
 *    else  = very bright
 */
const int thresholdLightIntensity = 200;
const uint32_t lengthLightOn = 180000; // 3 minutes

void switch_on();
void switch_off();

void setup() {
  pinMode(pinLightSwitch, OUTPUT);
  pinMode(pinMotionSensor, INPUT);
  pinMode(pinLightSensor, INPUT);
}

void loop() {
  int curLightIntensity = analogRead(pinLightSensor);
  motionDetected = (boolean)digitalRead(pinMotionSensor);

  if (motionDetected) {
    lastMotionTime = millis();
  }

  if (motionDetected && curLightIntensity < thresholdLightIntensity) {
    switch_on();
  }
  else {
    switch_off();
  }
}

// Switch on the light for a preset duration
// and check again for a motion after that 
void switch_on() {
  uint32_t curTime = millis();

  while (curTime - lastMotionTime < lengthLightOn) {
    if (stateLightSwitch == LOW) {
      stateLightSwitch = HIGH;
      digitalWrite(pinLightSwitch, stateLightSwitch);
    }

    motionDetected = (boolean)digitalRead(pinMotionSensor);

    if (motionDetected) {
      lastMotionTime = millis();
    }

    curTime = millis();
  }
}

// Switch off the light if it is on
void switch_off() {
  if (stateLightSwitch == HIGH) {
    stateLightSwitch = LOW;
    digitalWrite(pinLightSwitch, stateLightSwitch);
  }
}
