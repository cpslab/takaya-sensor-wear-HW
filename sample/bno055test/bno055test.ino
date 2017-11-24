int interrupt;
void setup() {
  // put your setup code here, to run once:
  pinMode(13, INPUT);
  Serial.begin(115200);
  interrupt = 0;
  Serial.println("START TEST");
  Serial.println(digitalRead(13));
}

void loop() {
  // put your main code here, to run repeatedly:
  int temp = digitalRead(13);
  if(interrupt != temp){
    interrupt = temp;
    Serial.println(interrupt);
  }
  
}
