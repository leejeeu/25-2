int ledPeriod;
int ledDuty;

void set_period(int period) {
  ledPeriod = period;
}

void set_duty(int duty) {
  ledDuty = duty;
}

int set_on() {
  return ledPeriod * ledDuty / 100;
}

int set_off() {
  return ledPeriod - set_on();
}

void setup() {
  pinMode(7, OUTPUT);
}

void loop() {
  set_period(1000);

  // 밝아짐
  for (int i = 0; i <= 100; i++) {
    set_duty(i);
    digitalWrite(7, HIGH);
    delayMicroseconds(set_on());
    digitalWrite(7, LOW);
    delayMicroseconds(set_off());
  }

  // 어두워짐
  for (int i = 100; i >= 0; i--) {
    set_duty(i);
    digitalWrite(7, HIGH);
    delayMicroseconds(set_on());
    digitalWrite(7, LOW);
    delayMicroseconds(set_off());
  }
}
