#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// LCD Setup (I2C address, columns, rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DHT Sensor Setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// MQ2 Gas Sensor
#define MQ2PIN A0

// Buzzer pin
#define BUZZER 8

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();

  // Serial Monitor
  Serial.begin(9600);

  // Start DHT sensor
  dht.begin();

  // Buzzer pin as output
  pinMode(BUZZER, OUTPUT);

  // Welcome message
  lcd.setCursor(0, 0);
  lcd.print("Air Quality Mon");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read DHT values
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read gas sensor (MQ2)
  int gasValue = analogRead(MQ2PIN);

  // Handle sensor errors
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT11!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor error");
    delay(2000);
    return;
  }

  // Determine Air Quality from MQ2 gas value
  String airQuality;
  if (gasValue < 300) {
    airQuality = "FRESH";
    digitalWrite(BUZZER, LOW);  // Buzzer OFF
  } else if (gasValue < 600) {
    airQuality = "MODERATE";
    digitalWrite(BUZZER, LOW);  // Buzzer OFF
  } else {
    airQuality = "POOR";
    digitalWrite(BUZZER, HIGH); // Buzzer ON
  }

  // Print to Serial Monitor
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" C, Hum: ");
  Serial.print(humidity);
  Serial.print(" %, Gas: ");
  Serial.print(gasValue);
  Serial.print(" -> Air: ");
  Serial.println(airQuality);

  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("C H:");
  lcd.print(humidity, 0);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Gas:");
  lcd.print(gasValue);
  lcd.print(" ");
  lcd.print(airQuality);

  delay(3000);
}
