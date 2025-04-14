#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BlynkSimpleEsp32.h>
 
 
char auth[] = "YOUR_AUTH_TOKEN";
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";
 
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
// DTH11 Sensor
#define DHTPIN 18
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
 
// DS18B20 Sensor
#define ONE_WIRE_BUS 19
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
 
#define VIRTUAL_PIN_TEMP BLYNK_VIRTUAL_PIN_START
#define VIRTUAL_PIN_HUMIDITY BLYNK_VIRTUAL_PIN_START + 1
 
void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.setBacklight(LOW);
  dht.begin();
  sensors.begin();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
 
 
  Blynk.begin(auth, ssid, pass);
}
 
void loop() {
  Blynk.run();
  float temperature = readDTH11Temperature();
  float humidity = readDTH11Humidity();
  float dsTemperature = readDS18B20Temperature();
 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
 
  Blynk.virtualWrite(VIRTUAL_PIN_TEMP, temperature);
  Blynk.virtualWrite(VIRTUAL_PIN_HUMIDITY, humidity);
  delay(1000);
}
 
float readDTH11Temperature() {
  float temperature = dht.readTemperature();
  return temperature;
}
 
float readDTH11Humidity() {
  float humidity = dht.readHumidity();
  return humidity;
}
 
float readDS18B20Temperature() {
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  return temperature;
}
