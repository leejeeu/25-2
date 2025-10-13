#include <Servo.h>

#define PIN_TRIG 12
#define PIN_ECHO 13
#define PIN_SERVO 4
#define PIN_LED 5

#define SND_VEL 346.0
#define INTERVAL 25
#define PULSE_DURATION 10
#define _DIST_MIN 180.0
#define _DIST_MAX 360.0

#define TIMEOUT ((INTERVAL/2)*1000.0)
#define SCALE (0.001*0.5*SND_VEL)

#define _EMA_ALPHA 0.3

#define _DUTY_MIN 1000   // 안전 범위
#define _DUTY_NEU 1500
#define _DUTY_MAX 2000   // 안전 범위

float dist_ema = _DIST_MAX, dist_prev = _DIST_MAX;
unsigned long last_sampling_time = 0;

Servo servo;

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH); // LED OFF 초기화

  servo.attach(PIN_SERVO);
  servo.writeMicroseconds(_DUTY_NEU);

  Serial.begin(57600);
}

void loop() {
  if(millis() < last_sampling_time + INTERVAL) return;

  float dist_raw = USS_measure(PIN_TRIG, PIN_ECHO);
  float dist_filtered;

  // 거리 필터링
  if((dist_raw == 0.0) || (dist_raw > _DIST_MAX)){
    dist_filtered = dist_prev;
    digitalWrite(PIN_LED, HIGH);
  } else if(dist_raw < _DIST_MIN){
    dist_filtered = dist_prev;
    digitalWrite(PIN_LED, HIGH);
  } else {
    dist_filtered = dist_raw;
    dist_prev = dist_raw;
    digitalWrite(PIN_LED, LOW);
  }

  // EMA 적용
  dist_ema = _EMA_ALPHA * dist_filtered + (1 - _EMA_ALPHA) * dist_ema;

  // map 적용 + 서보 및 LED 제어
  int duty;
  if(dist_filtered <= _DIST_MIN){
    duty = _DUTY_MIN;
    
  } else if(dist_filtered > _DIST_MIN && dist_filtered < _DIST_MAX){
    duty = map((int)dist_filtered, 180, 360, _DUTY_MIN, _DUTY_MAX);
    
  } else {
    duty = _DUTY_MAX;
  }

  servo.writeMicroseconds(duty);

  // 디버그 출력
  Serial.print("dist_raw:"); Serial.print(dist_raw);
  Serial.print(", dist_filtered:"); Serial.print(dist_filtered);
  Serial.print(", duty:"); Serial.println(duty);

  last_sampling_time += INTERVAL;
}

float USS_measure(int TRIG, int ECHO){
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;
}
