void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  Serial.println("Hello World");
  while(!Serial){
    ;
  }
  Serial.println("Hellp World!");
}
int count=0; int toggle =0;

void loop() {
  Serial.println(++count);
  toggle = toggle_state(toggle);
  digitalWrite(13, toggle);
  delay(1000);
}
int toggle_state(int toggle){
  return !toggle;
}
