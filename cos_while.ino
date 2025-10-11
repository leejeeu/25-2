#include <Servo.h>
#include <math.h>

#define PIN_TRIG 12
#define PIN_ECHO 13
#define PIN_SERVO 10

#define THRESHOLD_DISTANCE 20.0  // cm
#define MOVING_TIME 3000         // 차단기 이동 시간 (ms)

Servo gateServo;

int startAngle = 0;
int stopAngle  = 90;
int currentAngle = startAngle;
bool isCarDetected = false;

void setup() {
  Serial.begin(9600);
  gateServo.attach(PIN_SERVO);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  gateServo.write(startAngle); // 초기 상태: 닫힘
  delay(500);
  Serial.println("Gate system ready 🚧");
}

// 초음파 거리 측정 함수
float measureDistance() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duration = pulseIn(PIN_ECHO, HIGH);
  return duration * 0.034 / 2.0; // cm 단위 변환
}

void loop() {
  float distance = measureDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // 차량 접근 시 (차단기 상승)
  if (distance < THRESHOLD_DISTANCE && !isCarDetected) {
    unsigned long moveStartTime = millis();
    while (millis() - moveStartTime <= MOVING_TIME) {
      // while 루프 안에서 distance 재측정
      distance = measureDistance();

      // 차량가 떠나면 루프 탈출
      if (distance > THRESHOLD_DISTANCE + 10) {
        break;
      }

      float t = float(millis() - moveStartTime) / MOVING_TIME; // 0~1
      float ease = (1 - cos(M_PI * t)) / 2.0;                  // 코사인 절반 주기
      currentAngle = startAngle + (stopAngle - startAngle) * ease;
      gateServo.write(currentAngle);
      delay(20);
    }
    isCarDetected = true;
  }

  // 차량 떠난 후 (차단기 하강)
  if ((distance > THRESHOLD_DISTANCE + 10) && isCarDetected) {
    unsigned long moveStartTime = millis();
    while (millis() - moveStartTime <= MOVING_TIME) {
      distance = measureDistance(); // 하강 중에도 distance 체크

      // 차량가 다시 접근하면 루프 탈출
      if (distance < THRESHOLD_DISTANCE) {
        break;
      }

      float t = float(millis() - moveStartTime) / MOVING_TIME; // 0~1
      float ease = (1 - cos(M_PI * t)) / 2.0;
      currentAngle = stopAngle - (stopAngle - startAngle) * ease;
      gateServo.write(currentAngle);
      delay(20);
    }
    isCarDetected = false;
  }
}
