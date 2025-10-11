#include <Servo.h>
#include <math.h>

#define TRIG 12
#define ECHO 13
#define PIN_SERVO 10

Servo myServo;

// ------------------------------
// 변수 정의
// ------------------------------
unsigned long MOVING_TIME = 2000; // 2초 동안 이동
unsigned long moveStartTime;
int startAngle = 30;
int stopAngle  = 90;

bool carDetected = false; // 차량 감지 상태

// ------------------------------
// 거리 측정 함수
// ------------------------------
float getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  float distance = duration * 0.034 / 2; // cm
  return distance;
}

// ------------------------------
// setup
// ------------------------------
void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  myServo.attach(PIN_SERVO);
  myServo.write(startAngle);
  delay(500);
  moveStartTime = millis();
}

// ------------------------------
// loop
// ------------------------------
void loop() {
  float distance = getDistance();
  Serial.print("거리: ");
  Serial.print(distance);
  Serial.println(" cm");

  // 차량 감지 기준 (20cm 이내)
  if (distance < 20 && !carDetected) {
    carDetected = true;
    startAngle = 30;
    stopAngle = 90;
    moveStartTime = millis();
  } else if (distance >= 20 && carDetected) {
    carDetected = false;
    startAngle = 90;
    stopAngle = 30;
    moveStartTime = millis();
  }

  // 부드러운 제어 (Sigmoid 함수)
  unsigned long progress = millis() - moveStartTime;
  float t = (float)progress / MOVING_TIME; // 0~1
  if (t <= 1.0) {
    float k = 10.0; // 기울기
    float sigmoid = 1.0 / (1.0 + exp(-k * (t - 0.5))); // sigmoid 계산
    int angle = startAngle + (stopAngle - startAngle) * sigmoid;
    myServo.write(angle);
  }

  delay(50);
}
