int ledPeriod = 100;
int ledDuty = 0;

void setup() {
  pinMode(7, OUTPUT);
}

void set_period(int period) {
  ledPeriod = period;
}

void set_duty(int duty) {
  if(duty < 0) duty = 0;
  if(duty > 100) duty = 100;
  ledDuty = duty;
}

void loop() {
  set_period(100);

  for(int i=0; i<=100; i++) {
    set_duty(i);
    int onTime = ledPeriod * ledDuty / 100;
    int offTime = ledPeriod - onTime;

    digitalWrite(7, HIGH);
    delayMicroseconds(onTime);
    digitalWrite(7, LOW);
    delayMicroseconds(offTime);
  }

  for(int i=100; i>=0; i--) {
    set_duty(i);
    int onTime = ledPeriod * ledDuty / 100;
    int offTime = ledPeriod - onTime;

    digitalWrite(7, HIGH);
    delayMicroseconds(onTime);
    digitalWrite(7, LOW);
    delayMicroseconds(offTime);
  }
}
