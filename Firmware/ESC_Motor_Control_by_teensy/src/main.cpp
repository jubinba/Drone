#include <Arduino.h>
#include<PulsePosition.h>
// Connect ppm pin of reciever to pin 14 of Teensy(which is also ppm)
// Connect the power and ground of reciever to 5v teensy output and ground
// pwm frequency of reciever and esc is 4 millisecond or 250 hz(The signals will repeat every milliseconds)
// If we are using PWM then we need to use seperate signals(roll,ptich,yaw)hence we need to use seperate pins in the teensy which can cause time delays ande we have to do corrections.
// In PPM we are using position and not the width and the pulses are placed after each other and they can send all signals at a single line

PulsePositionInput RecieverInput(RISING);         // Create a ppm input object(Reciever Input) && we are tracking each pulse starting from the risisng edge
float RecieverValue[] = {0, 0, 0, 0, 0, 0, 0, 0}; // Array for storing upto 8 channel values
int ChannelNumber = 0;                            // For storing the channel number

float InputThrottle; // This value lies betweeen 1000 ans 2000us. This is used for a condition for not accidently turn un the motor for startup

// Creating as a function for reading the Wireless Reciver output for the throttle input by the transmitter
void read_reciever(void){
  // checking how many channels are available
  ChannelNumber = RecieverInput.available();   // myInput.available(); ->Returns the number of channels available(our case 3 RPY), or zero when no new data is available.
                                               // Here we are checking the cahnnel number and asssigning it to the variable ChannelNumber
 
  if(ChannelNumber>0)
  {
    // If we are in the first channel
    // Reading values of each channel into the array
    // myInput.read(channel)->The returned value is a float representing the number of microseconds between rising edges.
    for (int i = 0; i < ChannelNumber; i++)
    {
      RecieverValue[i - 1] = RecieverInput.read(i);
    }
    }
  }






void setup() {

  Serial.begin(57600);
  pinMode(13,OUTPUT);         // Teensy inbuilt led
  digitalWrite(13,HIGH);
  RecieverInput.begin(14);   // myInput.begin(rxPin); -> Assign an input to a pin and begin receiving pulses.
  analogWriteFrequency(1,250); // Send PWM signal from pin 1 of teenzy with a frequency of 250 Hz.
  analogWriteResolution(12);
  delay(250);

  // Creating a loop to avoid uncontrolled motor start(so that motor wont start if the throtthle is accidently in full position)
  // The motor will only start if throttle is between 1020 and 1050 otherwise it will get repeated in this loop and wont go to void loop

  while (RecieverValue[2]<1020||RecieverValue[2]>1050)
  {
    read_reciever();
    delay(4);
  }
  
  
}

void loop() {
  read_reciever();
  InputThrottle = RecieverValue[2];
  analogWrite(1,InputThrottle*1.024);
 
}

