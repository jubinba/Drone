#include <Arduino.h>
#include <PulsePosition.h>
// Connect ppm pin of reciever to pin 14 of Teensy(which is also ppm)
// Connect the power and ground of reciever to 5v teensy output and ground
// pwm frequency of reciever and esc is 4 millisecond or 250 hz(The signals will repeat every milliseconds)
// If we are using PWM then we need to use seperate signals(roll,ptich,yaw)hence we need to use seperate pins in the teensy which can cause time delays ande we have to do corrections.
// In PPM we are using position and not the width and the pulses are placed after each other and they can send all signals at a single line

PulsePositionInput RecieverInput(RISING);         // Create a ppm input object(Reciever Input) && we are tracking each pulse starting from the risisng edge
float RecieverValue[] = {0, 0, 0, 0, 0, 0, 0, 0}; // Array for storing upto 8 channel values
int ChannelNumber = 0;                            // For storing the channel number

// Creating as a function for reading multiple times

void read_reciever(void)
{
    // checking how many channels are available
    ChannelNumber = RecieverInput.available(); // myInput.available(); ->Returns the number of channels available(our case 3 RPY), or zero when no new data is available.
                                               // Here we are checking the number of channels and asssigning it to the variable ChannelNumber

    if (ChannelNumber > 0)
    { // If we are in the first channel
        for (int i = 0; i < ChannelNumber; i++)
        {
            RecieverValue[i - 1] = RecieverInput.read(i); // Reading calues of each channel into the array
                                                          // myInput.read(channel)->The returned value is a float representing the number of microseconds between rising edges.
        }
    }
}

void setup()
{
    Serial.begin(57600);
    pinMode(13, OUTPUT); // Teensy inbuilt led
    digitalWrite(13, HIGH);
    RecieverInput.begin(14); // myInput.begin(rxPin); -> Assign an input to a pin and begin receiving pulses.
}

void loop()
{   /// PPM signals corresponding to the Remote Control Inputs from the throttles
    read_reciever();
    Serial.print("PPM values correspoding to the throttle inputs :");
    Serial.println("Numer of Channels:");
    Serial.print(ChannelNumber);
    Serial.print("Roll[us]:");
    Serial.print(RecieverValue[0]);
    Serial.print("Pitch[us]:");
    Serial.print(RecieverValue[1]);
    Serial.print("Yaw[us]:");
    Serial.println(RecieverValue[2]);
    delay(50);

}
