// C++ code
//
#define INTERVAL 25
int distime=0; 
float distance=0;
void setup()
{
  pinMode(9, OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,INPUT);
  Serial.begin(9600);
}
int brightness(void){
  int i;
  if(200<distance && distance<=300)  i=(distance-200)*255/100;
  else if(100<=distance && distance<=200) i=(200-distance)*255/100;
  else i=255;
  return i;
}


void measure(void){
  digitalWrite(12,LOW);
  delayMicroseconds(2);
  digitalWrite(12,HIGH);
  delayMicroseconds(10);
  digitalWrite(12,LOW);
  
  distime = pulseIn(13,HIGH,30000);
  if(distime ==0) distance = 0;
  else distance = (float) distime / 58.2*10;
 
  Serial.print("거리:"); Serial.print(distance);
  Serial.print("밝기:"); Serial.print(brightness());
  Serial.print("");

  Serial.print(distance);
  Serial.print("\t");
  Serial.println(brightness());
}
 
unsigned long last_sampling_time=0;

void loop()
{
  
  if(millis()< last_sampling_time+INTERVAL){
    return;
  }
  measure();
  
  analogWrite(9, brightness());
  last_sampling_time += INTERVAL ;
       
    
}
