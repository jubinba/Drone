#include <Arduino.h>
#include<Wire.h>

float RateRoll,RatePitch,RateYaw;
float RateCalibrationRoll,RateCalibrationPitch,RateCalibrationYaw;//Calibration variables for the roll,pitch and yaw rotation rates
int RateCalibrationNumber;//Variable to keep track of number of values already recorded to do the calibration
void gyro_signals(void){             // Function created for I2c Communication

Wire.beginTransmission(0x68);       // Starting Transmission between teensy and MPU 6050 (Register Map - Who am I - default register address)
                                    // Selection of the inbuilt lowpass filter with cutoff frequency 10 Hz (Register Map and Descriptions ) or Motor rotations will cause vibrations and variations in readings
Wire.write(0x1A);
Wire.write(0x05);                   // Selecting the cutoff frequency
Wire.endTransmission();
// Setting up the sensitivity  scale factor of the gyroscope
Wire.beginTransmission(0x68);
Wire.write(0X1B);
Wire.write(0x8);                      // FSR 65.5LSB / (°/s)
Wire.endTransmission();
// Now we can import measurement values from the gyro
//these measurements are stored innthe registers with address 0x43 to 0x48
// Also we have to request 6 bytes from the MPU 6050 to pull the information from 6 registers(43 to 48)
// The measurement values of the gyroscope(gyroscope measurement registers and accelarometer measurement registers ) are stored in the register 43 - 48 
Wire.beginTransmission(0x68);
Wire.write(0x43);
Wire.endTransmission();

Wire.requestFrom(0x68,6);               // Request 6 bytes(x-2bytes,y-2bytes,z-3bytes) from GYRO_XOUT to pull the information from the 6 registers(43-48) from the sensor(here 68 represents the MPU 6050 ID and nott the Register Decimal)
                                          // Registers 43 and 44 contain rotation rate measurements in the LSB for X axis
                                          //As per datasheet these measurements are represented as signed 16 bit measurement. 
                                          // As the measurements around each axis is spread out over two registers(16 bit - 8 bit higher and 8 bit lower)This has to be merged by calling Wire.read() twice.
int16_t GyroX = Wire.read()<<8|Wire.read(); // Read the gyro measurements from X axis
int16_t GyroY = Wire.read()<<8|Wire.read(); // Y axis
int16_t GyroZ = Wire.read()<<8|Wire.read();// Z axis

RateRoll = (float)GyroX/65.5;              // Convert the measurement in LSB to °/s
RatePitch = (float)GyroY/65.5;              //65.5 is used because earlier we selected that as the the sensitivity factor
RateYaw = (float)GyroZ/65.5;



}





void setup() {

  Serial.begin(57600);
  pinMode(13,OUTPUT);               
  digitalWrite(13,HIGH);
  Wire.setClock(400000); // Set clock speed of I2C (400KHz -mentioned in MPU 6050 product specifications) 
  Wire.begin(); //To initialise wire library
  delay(250);

  // To activate the MPU 6050 , write to the power management register(6B)
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00); // All the bits in this register has to be set to zero in order to start and continue in power mode.
  Wire.endTransmission();
  // BEGIN CALIBRATION
  //The sensor should be kept stationary while calibration
  // Here, we are basically finding the average of the Roll, pitch and yaw measurements
  for (RateCalibrationNumber = 0; RateCalibrationNumber < 2000; RateCalibrationNumber++)// The cal. takes place in 2000 step

  {
    gyro_signals();
    RateCalibrationRoll+=RateRoll;
    RateCalibrationPitch+=RatePitch;
    RateCalibrationYaw+=RateCalibrationPitch;
    delay(1);

  }

  RateCalibrationRoll/=2000;//(sum / no of steps -> For average)
  RateCalibrationPitch/=2000;// These are the Calibration Values(Values rhat were obtained when the measuremnts were supposed to be 0)
  RateCalibrationYaw/=2000;// Now we have to subtract these from the measurements recieved
  



}

void loop() {
  
  
  gyro_signals();
  RateRoll-=RateCalibrationRoll;  // Correcting the values
  RatePitch-=RateCalibrationPitch;
  RateYaw-=RateCalibrationYaw;
  Serial.print("Roll Rate[°/s]=");
  Serial.print(RateRoll);
  Serial.print("Pitch Rate[°/s]=");
  Serial.print(RatePitch);
  Serial.print("Yaw Rate[°/s]=");
  Serial.println(RateYaw);
  delay(50);


  
}

