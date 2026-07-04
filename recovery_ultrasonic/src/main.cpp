#include <Arduino.h>

const int trigPinOne = 9;
const int echoPinOne = 10;
const int trigPinTwo = 11;
const int echoPinTwo = 12;
const int trigPinThree = 2;
const int echoPinThree = 3;
long duration;
int distance;
int distanceOne;
int distanceTwo;
int distanceThree;

double readUltrasoundSensorOne();
double readUltrasoundSensorTwo();
double readUltrasoundSensorThree();

void setup() {
    // put your setup code here, to run once:
    pinMode(trigPinOne, OUTPUT);  // Sets the trigPin as an Output
    pinMode(echoPinOne, INPUT);   // Sets the echoPin as an Input
    pinMode(trigPinTwo, OUTPUT);  // Sets the trigPin as an Output
    pinMode(echoPinTwo, INPUT);
    pinMode(trigPinThree, OUTPUT);  // Sets the trigPin as an Output
    pinMode(echoPinThree, INPUT);
    Serial.begin(9600);
}

void loop() {
    distanceOne = readUltrasoundSensorOne();
    delay(30);
    distanceTwo = readUltrasoundSensorTwo();
    delay(30);
    distanceThree = readUltrasoundSensorThree();

    // Bulletproof way to find the absolute minimum of the three
    distance = distanceOne;
    if (distanceTwo < distance) {
        distance = distanceTwo;
    }
    if (distanceThree < distance) {
        distance = distanceThree;
    }

    // Just print the lowest distance alone! No extra text.
    Serial.println(distance);
    delay(30);
}

double readUltrasoundSensorOne() {
    digitalWrite(trigPinOne, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinOne, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinOne, LOW);
    duration = pulseIn(echoPinOne, HIGH);
    distance = duration * 0.034 / 2;
    return distance;
}

double readUltrasoundSensorTwo() {
    digitalWrite(trigPinTwo, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinTwo, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinTwo, LOW);
    duration = pulseIn(echoPinTwo, HIGH);
    distance = duration * 0.034 / 2;
    return distance;
}

double readUltrasoundSensorThree() {
    digitalWrite(trigPinThree, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinThree, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinThree, LOW);
    duration = pulseIn(echoPinThree, HIGH);
    distance = duration * 0.034 / 2;
    return distance;
}
