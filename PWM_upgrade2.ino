void setup() {
  pinMode(7,OUTPUT);

}
float ledPeriod;
float ledDuty;
void set_period(float period){
  ledPeriod = period;
}
void set_duty(float duty){
  ledDuty =  duty/100.0;
}
float set_on(){
  return ledPeriod*ledDuty;
}
float set_off(){
  return ledPeriod-set_on();
}
void loop() {
  set_period(10000);
  for(int i=0; i<=100; i++){
    set_duty(i);
    digitalWrite(7,HIGH);
    delayMicroseconds((int)set_on());
    digitalWrite(7,LOW);
    delayMicroseconds((int)set_off());
  }
  for(int i=100; i>=0; i--){
    set_duty(i);
    digitalWrite(7,HIGH);
    delayMicroseconds((int)set_on());
    digitalWrite(7,LOW);
    delayMicroseconds((int)set_off());
  }

}
