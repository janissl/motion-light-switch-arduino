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
  lastMotionTime = millis();
}

void loop() {
  int curLightIntensity = analogRead(pinLightSensor);
  uint32_t curTime = millis();

  if ((boolean)digitalRead(pinMotionSensor)) {
    lastMotionTime = millis();
  }

  while (curLightIntensity < thresholdLightIntensity && curTime - lastMotionTime < lengthLightOn) {
     switch_on();

     curLightIntensity = analogRead(pinLightSensor);

     if ((boolean)digitalRead(pinMotionSensor)) {
      lastMotionTime = millis();
     }

     curTime = millis();
  }

  switch_off();
}

// Switch on the light if it is off 
void switch_on() {
    if (stateLightSwitch == LOW) {
      stateLightSwitch = HIGH;
      digitalWrite(pinLightSwitch, stateLightSwitch);
    }
}

// Switch off the light if it is on
void switch_off() {
  if (stateLightSwitch == HIGH) {
    stateLightSwitch = LOW;
    digitalWrite(pinLightSwitch, stateLightSwitch);
  }
}
