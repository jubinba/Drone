#include <Arduino.h>
#include <Wire.h>

// -----------------------------------------------gyro variables-----------------------------------------------------------------------------------------------------

float RateRoll, RatePitch, RateYaw;
float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw; // Calibration variables for the roll,pitch and yaw rotation rates
int RateCalibrationNumber;                                           // Variable to keep track of number of values already recorded to do the calibration(for finding the avverate)

//..............................................Reciever Variables...................................................................................................

#include <PulsePosition.h>
PulsePositionInput RecieverInput(RISING);         // Create a ppm input object(Reciever Input) && we are tracking each pulse starting from the risisng edge
float RecieverValue[] = {0, 0, 0, 0, 0, 0, 0, 0}; // Array for storing upto 8 channel values
int ChannelNumber = 0;                            // For storing the channel number

//---------------------------------------------Power_Switch_Variables(BMS)--------------------------------------------------------------------------------------------------

float Voltage, Current, BatteryATStart, BatteryRemaining; // Declaring variables as a floating point number
float BatteryDefault = 1300;                              // Default battery capacity 1300mah
float CurrentConsumed = 0;

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t LoopTimer; // Declaring a vriable LOOPTimer of unsigned 32 bit integer (this parameter contains the length of each control loop)

//-------------------------------------------PID_Controller_Variables-----------------------------------------------------------------------------------------------------------

float DesiredRateRoll, DesiredRatePitch, DesiredRateYaw;
float ErrorRateRoll, ErrorRatePitch, ErrorRateYaw;
float InputRoll, InputPitch, InputYaw, InputThrottle;
float PrevErrorRateRoll, PrevErrorRatePitch, PrevErrorRateYaw;
float PIDReturn[] = {0, 0, 0};
float PRateRoll = 0.6, PRatePitch = PRateRoll, PRateYaw = 2;
float IRateRoll = 3.5, IRatePitch = IRateRoll, IRateYaw = 12;
float DRateRoll = 0.03, DRatePitch = DRateRoll, DRateYaw = 0;

//------------------------------------------------MotorInputVariables-----------------------------------------------------------------------------------------------------------

float MotorInput1,MotorInput2,MotorInput3,MotorInput4;

//..................................................Creating_Necessary_functions...................................................................................................

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
}

void loop()
{
}
