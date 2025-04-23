#include <Arduino.h>

float Voltage, Current, BatteryATStart,BatteryRemaining; // Declaring variables as a floating point number
float BatteryDefault = 1300;            // Default battery capacity 1300mah
float CurrentConsumed = 0;

// Creating a function for measuring battery voltage

void battery_voltage(void)
{
  // Measuring Voltage and current of the battery
  Voltage = (float)analogRead(15) / 62;    // Reading analog voltage from pin 15
                                           // Default resolution of analogRead is 10 bit
                                           // Also we are using a 1/5 Voltage divider(1023/(3.3*5))=62
  Current = (float)analogRead(21) * 0.089; // Default resolution for analog read is 10 bits(0 to 1023)
                                           // for one volt 1023/3.3
                                           // Also since the load cureennt ratio is o.o36 V/A
                                           //((1023/3.3 )*0.036 = 0.089)
}

void setup()
{

  // Determining ht battery capacity suring startup only prior flight(throttle)
  // Serial.begin(57600);  baude rate-The speed by which the teensy communicates with pc (57600 bits persec) // This is only required when we are using Serial.print() or any serial communication commands

  digitalWrite(5, HIGH); // RED LED turned on

  battery_voltage();

  if (Voltage > 8.3)
  {
    digitalWrite(5, LOW);            // Turn off the red LED
    BatteryATStart = BatteryDefault; // Assigning BatteryATStart as 1300maH
  }

  else if (Voltage < 7.5)
  {
    BatteryATStart = 30 / 100 * BatteryDefault; // Assigning BatteryATStart as 30% of 1300maH and the RED led is blinking
  }

  else
  {
    BatteryATStart = (82 * Voltage - 580) / 100 * BatteryDefault; // Relation between battery capacity and voltage derived using linear regression & RED LED is on
  }
}

void loop()
{
  battery_voltage();
  CurrentConsumed = Current*1000*0.004/3600+CurrentConsumed; // converting ampere to milliampere and seconds to hours
  BatteryRemaining = (BatteryATStart-CurrentConsumed)/BatteryDefault*100;
  if (BatteryRemaining<=30){
    digitalWrite(5,HIGH);
  }
  else
  digitalWrite(5,LOW);

}