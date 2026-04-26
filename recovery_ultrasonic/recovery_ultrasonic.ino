// recovery_ultrasonic.ino
// Reads three HC-SR04 ultrasonic sensors and prints all three readings
// over serial as CSV: "s1,s2,s3\r\n"
// Compatible with Arduino IDE's Serial Plotter.

const int trigPinOne   = 9;
const int echoPinOne   = 10;
const int trigPinTwo   = 11;
const int echoPinTwo   = 12;
const int trigPinThree = 2;
const int echoPinThree = 3;

// Cap pulseIn at 30 ms -> roughly 5 m max range. Anything longer is treated
// as "no echo" so the loop never stalls for a full second on a missing reading.
const unsigned long PULSE_TIMEOUT_US = 30000UL;

// Returned when a sensor times out (no echo within PULSE_TIMEOUT_US).
const int OUT_OF_RANGE_CM = 999;

void setup() {
  pinMode(trigPinOne, OUTPUT);
  pinMode(echoPinOne, INPUT);
  pinMode(trigPinTwo, OUTPUT);
  pinMode(echoPinTwo, INPUT);
  pinMode(trigPinThree, OUTPUT);
  pinMode(echoPinThree, INPUT);
  Serial.begin(9600);
}

void loop() {
  int distanceOne   = readUltrasoundSensor(trigPinOne,   echoPinOne);
  delay(30);
  int distanceTwo   = readUltrasoundSensor(trigPinTwo,   echoPinTwo);
  delay(30);
  int distanceThree = readUltrasoundSensor(trigPinThree, echoPinThree);

  // CSV output: sensor1, sensor2, sensor3
  Serial.print(distanceOne);
  Serial.print(',');
  Serial.print(distanceTwo);
  Serial.print(',');
  Serial.println(distanceThree);

  delay(200);
}

// Single reader function — pass in whichever trig/echo pair you want to read.
// Returns distance in cm, or OUT_OF_RANGE_CM if no echo arrives in time.
int readUltrasoundSensor(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, PULSE_TIMEOUT_US);
  if (duration == 0) {
    return OUT_OF_RANGE_CM;  // timeout: nothing in range
  }
  // 0.0343 cm/us speed of sound, divided by 2 for round-trip.
  return (int)(duration * 0.0343 / 2.0);
}
