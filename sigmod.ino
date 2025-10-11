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
int startAngle = 0;
int stopAngle  = 90;

bool carDetected = false; // 차량 감지 상태
float prevDistance = 100.0; // 이동평균용 이전 거리

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
  // 간단 이동평균 필터
  distance = (distance + prevDistance) / 2;
  prevDistance = distance;
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

  // 히스테리시스 적용: ON 15cm, OFF 20cm
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

  // 부드러운 제어 (Sigmoid 함수)
  unsigned long progress = millis() - moveStartTime;
  float t = (float)progress / MOVING_TIME; // 0~1

  int angle;
  if (t >= 1.0) {
    angle = stopAngle; // 완료 후 각도 고정
  } else {
    float k = 8.0; // 기울기 약간 낮춰서 부드럽게
    float sigmoid = 1.0 / (1.0 + exp(-k * (t - 0.5)));
    angle = startAngle + (stopAngle - startAngle) * sigmoid;
  }

  myServo.write(angle);
  delay(50);
}
