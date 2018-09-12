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
  boolean movementDetected = (boolean)digitalRead(pinMotionSensor);

  if (curLightIntensity < thresholdLightIntensity && movementDetected) {
    switch_on();
  }
  else {
    switch_off();
  }
}

// Switch on the light for a preset duration
// and check again for a motion after that 
void switch_on() {
  boolean continueLightOn = false;
  uint32_t startTime = millis();
  uint32_t curTime = startTime;

  while (curTime - startTime < lengthLightOn) {
    if (stateLightSwitch == LOW) {
      stateLightSwitch = HIGH;
      digitalWrite(pinLightSwitch, stateLightSwitch);
    }

    boolean movementDetected = (boolean)digitalRead(pinMotionSensor);

    if (movementDetected) {
      continueLightOn = true;
      break;
    }

    curTime = millis();
  }

  if (continueLightOn) {
    switch_on();
  }
}

// Switch off the light if it is on
void switch_off() {
  if (stateLightSwitch == HIGH) {
    stateLightSwitch = LOW;
    digitalWrite(pinLightSwitch, stateLightSwitch);
  }
}
