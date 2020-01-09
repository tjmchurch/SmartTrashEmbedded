#include <Ultrasonic.h>
//download from
//https://www.arduinolibraries.info/libraries/ultrasonic
//Not standard
#include <Servo.h>
//lid automation
const int trigPin1 = 8;
const int echoPin1 = 10;
//height measurement
const int trigPin2 = 12;
const int echoPin2 = 11;

const int openServoPosition = 180;
const int closedServoPosition = 0;

const int servoPin = 9;
const int force0 = A0;
const int force1 = A1;
const int force2 = A2;
const int force3 = A3;

int height = 0;
int weight = 0;

Servo servo;

Ultrasonic ultrasonic1(trigPin1, echoPin1);
Ultrasonic ultrasonic2(trigPin2, echoPin2);


void setup() {
  Serial.begin(9600);

  servo.attach(servoPin);
  servo.write(closedServoPosition);
  //init servo, pins for ultrsonic,pins for force
  //close the lid

}

void loop() {
  if (isTriggered()) {
    openLid();
    collectData();
    Serial.println("height");
    Serial.println(height);
    Serial.println(readUltra(1));
  }
}

//function check if lid is triggered
//return true if lid needs to be open
bool isTriggered() {
  if (readUltra(2) < 15) {
    if (readUltra(2) < 15) {
      Serial.println(readUltra(2));
      return true;
    }
  }
  return false;
}
void openLid() {
  //open lid
  for (int pos = closedServoPosition; pos <= openServoPosition; pos += 1) {
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(3);                       // waits 15ms for the servo to reach the position
  }
  //wait 4 seconds
  delay(4000);
  //close lid
  for (int pos = openServoPosition; pos >= closedServoPosition; pos -= 1) {
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(3);                       // waits 15ms for the servo to reach the position
  }
}
void collectData() {
  int tmp = 50;
  int diff = 0;
  for (int i = 0; i < 100; i++) {
    int tmp2 = readUltra(1);
    if (tmp2 && i >49) {
      Serial.println(tmp2);
      tmp = tmp + tmp2;
      diff = diff + 1;
    }
    delay(5);
    }
  Serial.println(tmp);
  Serial.println(diff);
  tmp = tmp / diff;
  Serial.println(tmp);
  height = tmp;
  weight = readWeight();

  //send data
}
//read ultrasonic sensor in CM
//number is either 1 or 2, depending on the ultrasonic read
unsigned int readUltra(int number) {
  if (number == 1) {
    return ultrasonic1.read();
  } else {
    return ultrasonic2.read();
  }
}
//return the combine weight on the force sensors
//may need to subtract so weight for inner bucket and lid
unsigned int readWeight() {
  return analogRead(force0) + analogRead(force1) + analogRead(force2) + analogRead(force3);
}
