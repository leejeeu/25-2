#include <Servo.h>
#include <math.h>

#define PIN_TRIG 12
#define PIN_ECHO 13
#define PIN_SERVO 10

Servo myServo;

unsigned long MOVING_TIME = 3000;  // 서보 이동 시간 (ms)
int startAngle = 0;
int stopAngle  = 90;
int currentAngle = startAngle;
bool isCarDetected = false;
#define DIST_THRESHOLD 20.0  // cm

// 함수 선언
float getDistance();
float tanhEase(float t);  // tanh 기반 easing

void setup() {
  myServo.attach(PIN_SERVO);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  myServo.write(startAngle);
  delay(500);
}

void loop() {
  float distance = getDistance();

  // 차량 접근 시 (서보 상승)
  if (distance < DIST_THRESHOLD && !isCarDetected) {
    unsigned long moveStart = millis();
    while (millis() - moveStart <= MOVING_TIME) {
      float progress = float(millis() - moveStart) / MOVING_TIME;  // 0~1
      float rate = tanhEase(progress);  // 부드러운 easing
      currentAngle = startAngle + rate * (stopAngle - startAngle);
      myServo.write(currentAngle);
      delay(20);
    }
    isCarDetected = true;
  }

  // 차량 통과 후 (서보 하강)
  if (distance > DIST_THRESHOLD + 10 && isCarDetected) {
    unsigned long moveStart = millis();
    while (millis() - moveStart <= MOVING_TIME) {
      float progress = float(millis() - moveStart) / MOVING_TIME;  // 0~1
      float rate = tanhEase(progress);
      currentAngle = stopAngle - rate * (stopAngle - startAngle);
      myServo.write(currentAngle);
      delay(20);
    }
    isCarDetected = false;
  }
}

// tanh 기반 easing 함수
float tanhEase(float t) {
  // t: 0~1 → 중앙부에서 부드러운 기울기
  float x = (t - 0.5) * 5.0;      // 중앙부 확대, 스케일링
  return 0.5 * (tanh(x) + 1.0);   // -1~1 → 0~1 범위로 변환
}

// 초음파 거리 측정 함수
float getDistance() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duration = pulseIn(PIN_ECHO, HIGH);
  return duration * 0.0343 / 2.0;  // cm 단위 변환
}
