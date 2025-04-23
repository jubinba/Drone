
void setup() {
  
  pinMode(13, OUTPUT);   // Turn on the internal LED
  digitalWrite(13,HIGH);
  pinMode(5,OUTPUT);    // Turn on the red LED
  digitalWrite(5,HIGH);

  delay(4000);  // Wait 4 seconds

  digitalWrite(5,LOW);

  pinMode(6,OUTPUT);
  digitalWrite(6,HIGH);




}


void loop() {
                     
}
