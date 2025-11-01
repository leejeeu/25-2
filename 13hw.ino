#include <Servo.h>

#define D 1
#define PIN_SERVO 10

#define _DUTY_MIN 800
#define _DUTY_NEU 1700
#define _DUTY_MAX 2600
#define _POS_START (_DUTY_MIN + 100)
#define INTERVAL 20   // 20ms

const int _POS_END = (D == 1) ? (_DUTY_MAX - 100) : (_DUTY_NEU - 100);
const float _SERVO_SPEED = (D == 1) ? 3.0 : 0.3;

Servo myservo;

// Timing & position variables
unsigned long last_sampling_time = 0;
float duty_change_per_interval;  // how much to move per interval
float duty_curr;
int duty_target;



void setup() {
  myservo.attach(PIN_SERVO);

  // Start position
  duty_curr = duty_target = _POS_START;
  myservo.writeMicroseconds((int)duty_curr);

  // Calculate duty change per interval to match 60s per 180°
  // duty_change_per_interval = (total PWM range for 180°) * (speed / 180°) * (interval_sec)
  duty_change_per_interval =
    (_DUTY_MAX - _DUTY_MIN) * (_SERVO_SPEED / 180.0) * (INTERVAL / 1000.0);
}

void loop() {
  if (millis() - last_sampling_time < INTERVAL) return;
  last_sampling_time = millis();

  // Move servo toward target
  if (duty_target > duty_curr) {
    duty_curr += duty_change_per_interval;
    if (duty_curr > duty_target) duty_curr = duty_target;
  } else {
    duty_curr -= duty_change_per_interval;
    if (duty_curr < duty_target) duty_curr = duty_target;
  }

  myservo.writeMicroseconds((int)duty_curr);

  // Toggle target when end is reached
  if ((int)duty_curr == duty_target) {
    if (duty_target == _POS_START)
      duty_target = _POS_END;
    else
      duty_target = _POS_START;
  }
}
