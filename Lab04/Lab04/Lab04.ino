#include <Servo.h>
const int la = 220;
const int lbf = 233;
const int c = 261;
const int cs = 277;
const int d = 294;
const int e = 329;
const int f = 349;
const int fs = 370;
const int g = 391;
const int gf = 370;
const int a = 440;
const int b = 466;
const int bf = 466;

const int notes[]{
  0, g, fs, e, e,
  fs, 0,
  0, la, g, fs, e, e,
  fs, d, e, la,
  0, la,
  e, fs, g, e, cs,
  d, e, la, la,
  fs, 0,

  0, g, fs, e, e,
  fs, 0,
  0, la, g, fs, e, e,
  fs, d, e, la,
  0,

  e, fs, g, e, cs,
  d, e, la, d, e,
  f, e, d, c, 0,
  
  la, lbf,
  c, f, e, d, d, c,
  d, c, c, c, la, lbf,
  c, f, g, f, e, d,
  d, e, f, f, g, a,
  bf, bf, a, g, f, g,
  a, a, g, f, d, c,
  d, f, f, e, e, gf, gf
};
const double durations[]{
  2, 0.5, 0.5, 0.5, 0.5,
  2, 2,
  1.5, 0.5, 0.5, 0.5, 0.5, 1,
  1.5, 1, 0.5, 2.5,
  1.5, 0.5,
  1, 0.5, 1.5, 0.5, 1,
  1.5, 1, 0.5, 1,
  1.5, 2,

  2, 0.5, 0.5, 0.5, 0.5,
  2, 2,
  1.5, 0.5, 0.5, 0.5, 0.5, 1.5,
  0.5, 1.5, 0.5, 2.5,
  2,

  1, 0.5, 1.5, 0.5, 1.5,
  0.5, 1, 0.5, 0.5, 0.5,
  0.5, 0.5, 0.5, 0.5, 1,
  
  0.5, 0.5,
  1, 1, 0.5, 0.5, 0.5, 0.5,
  0.5, 0.5, 1, 1, 0.5, 0.5,
  1, 1, 0.5, 0.5, 0.5, 0.5,
  0.5, 0.5, 1, 1, 0.5, 0.5,
  0.5, 0.5, 1, 1, 0.5, 0.5,
  0.5, 0.5, 1, 1, 0.5, 0.5,
  0.5, 0.5, 0.5, 1, 0.5, 0.5, 1  
};

const int ledOnePin = 4;
const int ledTwoPin = 5;
const int ledThreePin = 6;
const int ledFourPin = 7;

const int leds[] = {
  ledOnePin,
  ledTwoPin,
  ledThreePin,
  ledFourPin
};

const int switchPin = 8;
const int speakerPin = 9;

unsigned long previousTime = 0;
int switchState = 0;
int prevSwitchState = 0;
int currentLed = 0;
long interval = 1500;

Servo myServo;
const int servoPin = 10;


void setup() {
  // put your setup code here, to run once:
  pinMode(ledOnePin, OUTPUT);
  pinMode(ledTwoPin, OUTPUT);
  pinMode(ledThreePin, OUTPUT);
  pinMode(ledFourPin, OUTPUT);

  pinMode(switchPin, INPUT);
  myServo.attach(servoPin);
}

void hourGlass(){
  while(true){
     unsigned long currentTime = millis();
  
    if (currentTime - previousTime > interval) {
      previousTime = currentTime;
      if (currentLed == 4) {
        return;
      }
      digitalWrite(leds[currentLed], HIGH);
      currentLed++;
    }
  
    switchState = digitalRead(switchPin);
  
    if (switchState != prevSwitchState) {
      digitalWrite(ledOnePin, LOW);
      digitalWrite(ledTwoPin, LOW);
      digitalWrite(ledThreePin, LOW);
      digitalWrite(ledFourPin, LOW);
  
      currentLed = 0;
  
      previousTime = currentTime;
    }
    prevSwitchState = switchState;  
  }
}


void song(){
  for(int i = 0; i < sizeof(notes) / sizeof(int); i++){
    int noteDuration = (1600.0 * (durations[i] / 4.0));
    tone(speakerPin, notes[i], noteDuration);
    digitalWrite(leds[i % 4], HIGH);
    delay(noteDuration * 1.3);
    noTone(speakerPin);
    digitalWrite(leds[i % 4], LOW);
  }
  return;
}

void loop() {
  // put your main code here, to run repeatedly:
//  digitalWrite(ledOnePin, HIGH);
//  digitalWrite(ledTwoPin, HIGH);
//  digitalWrite(ledThreePin, HIGH);
//  digitalWrite(ledFourPin, HIGH);

  hourGlass();

  digitalWrite(ledOnePin, LOW);
  digitalWrite(ledTwoPin, LOW);
  digitalWrite(ledThreePin, LOW);
  digitalWrite(ledFourPin, LOW);
  
  song();

  for(int i = 0; i < 2; i++){
  myServo.write(9000);
  delay(500);
  myServo.write(-9000);
  }
}
