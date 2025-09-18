int ledPeriod ;
int ledDuty ;

void setup() {
  pinMode(7, OUTPUT);
}

void set_period(int period) {
  ledPeriod = period;
}

void set_duty(int duty) {
  ledDuty = duty;
}

void loop() {
  set_period(100);

  for(int i=0; i<=100; i++) {
    set_duty(i);
    int on = ledPeriod * ledDuty / 100;
    int off = ledPeriod - on;

    digitalWrite(7, HIGH);
    delayMicroseconds(on);
    digitalWrite(7, LOW);
    delayMicroseconds(off);
  }

  for(int i=100; i>=0; i--) {
    set_duty(i);
    int on = ledPeriod * ledDuty / 100;
    int off = ledPeriod - on;

    digitalWrite(7, HIGH);
    delayMicroseconds(on);
    digitalWrite(7, LOW);
    delayMicroseconds(off);
  }
}
