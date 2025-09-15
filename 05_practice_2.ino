void setup() {
  pinMode(7,OUTPUT);

}
int i=5;
void loop() {
  while(i)
  {
    digitalWrite(7,HIGH);
    delay(1000);
    digitalWrite(7,LOW);
    delay(1000);
    i=i-1;
  }
}
