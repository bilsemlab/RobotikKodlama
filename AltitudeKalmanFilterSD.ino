/*
   https://github.com/LowPowerLab/SFE_BMP180
   https://github.com/denyssene/SimpleKalmanFilter
*/
#include <SimpleKalmanFilter.h>
#include <SFE_BMP180.h>

#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;

SimpleKalmanFilter pressureKalmanFilter(1, 1, 0.01);

SFE_BMP180 pressure;

const long SAVE_PERIOD = 100;
long save_time;

float baseline; // baseline pressure

double getPressure() {
  char status;
  double T, P;
  status = pressure.startTemperature();
  if (status != 0) {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0) {
      status = pressure.startPressure(3);
      if (status != 0) {
        delay(status);
        status = pressure.getPressure(P, T);
        if (status != 0) {
          return (P);
        }
      }
    }
  }
}

void setup() {

  Serial.begin(9600);

  // BMP180 Pressure sensor start
  if (!pressure.begin()) {
    Serial.println("BMP180 Pressure Sensor Error!");
    while (1); // Pause forever.
  }
  baseline = getPressure();

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
  }
  else
  {
    Serial.println("card initialized.");
  }

}

void loop() {

  float p = getPressure();
  float altitude = pressure.altitude(p, baseline);
  float estimated_altitude = pressureKalmanFilter.updateEstimate(altitude);
  String dataString = "";

  if (millis() > save_time) {
    Serial.print(altitude, 6);
    dataString += String(altitude) + ";";
    Serial.print(";");
    Serial.print(estimated_altitude, 6);
    dataString += String(estimated_altitude);
    Serial.println();
    save_time = millis() + SAVE_PERIOD;
    save_data(dataString);
  }

}


void save_data(String dataString)
{
  File dataFile = SD.open("datalog.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString+" kaydedildi");
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.csv");
  }
}
