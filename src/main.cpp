

// Libraries for SD card
#include <FS.h>
#include <SD.h>
#include <SPI.h>

#include <WiFi.h>
#include <time.h>
#include <Adafruit_BMP085.h>

#define SCLK 14
#define MISO  2
#define MOSI  15
#define CS  13


Adafruit_BMP085 bmp180;
String dataMessage;
SPIClass spi;


// Initialize SD card
void initSDCard(){

    spi.begin(SCLK, MISO, MOSI, CS);

  if (!SD.begin(CS,spi)) {
    Serial.println("Card Mount Failed");
    spi = SPIClass(HSPI);
    return;

  }

  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n",cardSize);
}

// Write to the SD card
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void initBMP180() {
if (!bmp180.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
}
}


void ReadTemperature() {
  float temperatureC = bmp180.readTemperature();
  String TempString = "Temp: " + String(temperatureC, 2) + "°C\r\n";
  Serial.println(TempString);
}



void setup() {
  Serial.begin(9600);
  initSDCard();
  initBMP180();
  
  // If the data.txt file doesn't exist
  // Create a file on the SD card and write the data labels
  File file = SD.open("/data.txt");
  if(!file) {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/data.txt", "Temperature Logger 256MB\r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file.close();
}

void loop() {


float temperatureC = bmp180.readTemperature();
String TempString = "Temp: " + String(temperatureC, 2) + "°C\r\n";
Serial.println(TempString);
appendFile(SD, "/data.txt", TempString.c_str());
delay(5000);

}


