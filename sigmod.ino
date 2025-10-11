#include <Servo.h>
#include <math.h>

#define TRIG 12
#define ECHO 13
#define PIN_SERVO 10

Servo myServo;


unsigned long MOVING_TIME = 2000; 
unsigned long moveStartTime;
int startAngle = 0;
int stopAngle  = 90;

bool carDetected = false; 
float prevDistance = 100.0; 


float getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  float distance = duration * 0.034 / 2; 
  // 간단 이동평균 필터
  distance = (distance + prevDistance) / 2;
  prevDistance = distance;
  return distance;
}


void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  myServo.attach(PIN_SERVO);
  myServo.write(startAngle);
  delay(500);
  moveStartTime = millis();
}

void loop() {
  float distance = getDistance();
  Serial.print("거리: ");
  Serial.print(distance);
  Serial.println(" cm");


  if (distance < 15 && !carDetected) {
    carDetected = true;
    startAngle = 30;
    stopAngle = 90;
    moveStartTime = millis();
  } 
  else if (distance > 20 && carDetected) {
    carDetected = false;
    startAngle = 90;
    stopAngle = 30;
    moveStartTime = millis();
  }


  unsigned long progress = millis() - moveStartTime;
  float t = (float)progress / MOVING_TIME; 

  int angle;
  if (t >= 1.0) {
    angle = stopAngle; 
  } else {
    float k = 8.0; 
    float sigmoid = 1.0 / (1.0 + exp(-k * (t - 0.5)));
    angle = startAngle + (stopAngle - startAngle) * sigmoid;
  }

  myServo.write(angle);
  delay(50);
}
