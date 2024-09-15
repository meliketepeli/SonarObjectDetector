#include <SoftwareSerial.h>
#include <Servo.h>
#include <TimerOne.h>

SoftwareSerial bluetooth(1, 0);
char t;
TimerOne timer;

#define trigPin 10
#define echoPin 11
#define ledPin 13
#define piezoPin 8

boolean toggle = LOW;
long duration, distance;

int notes[] = {262, 462, 862, 1662, 3262};
Servo myServo;

unsigned long previousMillis = 0;
const long interval = 10; 

void setup() {
  bluetooth.begin(9600);
  myServo.attach(12);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT); 
  pinMode(piezoPin, OUTPUT);
}

void ledCallback() {
  toggle = !toggle;
  digitalWrite(ledPin, toggle);
}

int calculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

void handleBeepAndLed(int distance) {
  if (distance > 40) {
    noTone(piezoPin);
    digitalWrite(ledPin, LOW);
  } else {
    if (distance <= 40 && distance > 30) {
      tone(piezoPin, notes[1]);
    } else if (distance <= 30 && distance > 20) {
      tone(piezoPin, notes[2]);
    } else if (distance <= 20 && distance > 10) {
      tone(piezoPin, notes[3]);
    } else {
      tone(piezoPin, notes[4]);
    }
  }
}

void loop() {
  unsigned long currentMillis = millis();

  if (bluetooth.available() > 0) {
    t = bluetooth.read();
  }

  distance = calculateDistance();
  handleBeepAndLed(distance);

  switch (t) {

    case 'S': 
      for (int i = 0; i <= 180; i++) {
        digitalWrite(ledPin, HIGH); 
        delay(10);
        myServo.write(i);
        delay(10);
        handleBeepAndLed(distance);
        delay(15);
      }
      break;

    case 'T':
      myServo.write(90);
      delay(10);
      digitalWrite(ledPin, LOW); 
      delay(10);
      handleBeepAndLed(distance);
      delay(15);
      break;

    case 'R': 
      myServo.write(90); 
      delay(10);
      for (int i = 0; i <= 180; i++) {
        digitalWrite(ledPin, HIGH); 
        delay(10);
        digitalWrite(ledPin, LOW);
        delay(20);
        myServo.write(i);
        delay(10);
        handleBeepAndLed(distance);
        delay(15); 
      }
      break;

    case 'Q': 
      myServo.write(90);
      delay(10);
      for (int i = 180; i >= 0; i--) {
        digitalWrite(ledPin, HIGH); 
        delay(10);
        digitalWrite(ledPin, LOW);
        delay(20);
        myServo.write(i);
        handleBeepAndLed(distance);
        delay(15);
      }
      break;

    case 'U': 
      for (int i = 0; i <= 45; i++) {
        digitalWrite(ledPin, HIGH); 
        delay(10);
        digitalWrite(ledPin, LOW);
        delay(20);
        myServo.write(i);
        handleBeepAndLed(distance);
        delay(15);
      }
      break;

    case 'O':
      for (int i = 0; i <= 90; i++) {
        digitalWrite(ledPin, HIGH); 
        delay(10);
        digitalWrite(ledPin, LOW);
        delay(20);
        myServo.write(i);
        handleBeepAndLed(distance);
        delay(15);
      }
      break;

    case 'P': 
      for (int i = 0; i <= 180; i++) {
        digitalWrite(ledPin, HIGH); 
        delay(10);
        digitalWrite(ledPin, LOW);
        delay(20);
        myServo.write(i);
        handleBeepAndLed(distance);
        delay(15);
      }
      break;

    case 'M': 
      timer.initialize(1000000); 
      timer.attachInterrupt(ledCallback);
      previousMillis = currentMillis; 
      if ((currentMillis - previousMillis) >= interval) {
      timer.detachInterrupt();
      digitalWrite(ledPin, LOW); 
  }
      break;
  }


}