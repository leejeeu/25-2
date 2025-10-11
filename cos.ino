#include <Servo.h>
#include <math.h>


#define PIN_TRIG 12
#define PIN_ECHO 13
#define PIN_SERVO 10


#define THRESHOLD_DISTANCE 20.0  // cm, 이 거리 이하로 접근 시 차단기 작동
#define MOVING_TIME 3000         // 차단기 이동 시간 (ms)

Servo gateServo;

int startAngle = 0;    // 내려간 상태
int stopAngle  = 90;   // 올라간 상태
bool isRaising = false;
bool isLowering = false;
unsigned long moveStartTime = 0;

void setup() {
  Serial.begin(9600);
  gateServo.attach(PIN_SERVO);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  gateServo.write(startAngle); // 초기 상태: 닫힘
  delay(500);
  Serial.println("Gate system ready 🚧");
}

float measureDistance() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duration = pulseIn(PIN_ECHO, HIGH);
  float distance = duration * 0.034 / 2.0; // cm 변환
  return distance;
}

void loop() {
  float distance = measureDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  unsigned long now = millis();

  // 차량 감지됨 (차단기 올라감)
  if (distance < THRESHOLD_DISTANCE && !isRaising && !isLowering) {
    isRaising = true;
    moveStartTime = now;
  }

  // 차량이 떠남 (차단기 내려감)
  if (distance > THRESHOLD_DISTANCE + 10 && !isLowering && !isRaising) {
    isLowering = true;
    moveStartTime = now;
  }

  // 차단기 동작 (코사인 기반)
  if (isRaising) {
    float t = float(now - moveStartTime) / MOVING_TIME; // 0~1
    if (t <= 1.0) {
      // 코사인 절반 주기 사용: 부드럽게 상승
      float ease = (1 - cos(M_PI * t)) / 2.0;
      int angle = startAngle + (stopAngle - startAngle) * ease;
      gateServo.write(angle);
    } else {
      gateServo.write(stopAngle);
      isRaising = false;
    }
  }

  if (isLowering) {
    float t = float(now - moveStartTime) / MOVING_TIME; // 0~1
    if (t <= 1.0) {
      // 코사인 절반 주기 사용: 부드럽게 하강
      float ease = (1 - cos(M_PI * t)) / 2.0;
      int angle = stopAngle - (stopAngle - startAngle) * ease;
      gateServo.write(angle);
    } else {
      gateServo.write(startAngle);
      isLowering = false;
    }
  }

  delay(100);
}
