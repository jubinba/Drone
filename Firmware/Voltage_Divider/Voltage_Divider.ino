float Voltage;  // Declaring voltage as a floating point number

// Creating a function for measuring battery voltage
void battery_voltage(void){
  Voltage = (float)analogRead(15)/62;//Reading analog voltage from pin 15
                                      // Default resolution of analogRead is 10 bit
                                      // Also we are using a 1/5 Voltage divider(1023/(3.3*5))=62
}



void setup() {
  Serial.begin(57600); // baude rate-The speed by which the teensy communicates with pc (57600 bits persec) 

  pinMode(13,OUTPUT);   // inbuilt led
  digitalWrite(13,HIGH);

}

void loop() {
  battery_voltage();
  Serial.print(Voltage);
  Seral.println("V");
  delay(50);    // measure the voltage every 50 millisecond


}
