#define BLYNK_TEMPLATE_ID "TMPL3h6aYhhhv"
#define BLYNK_TEMPLATE_NAME "project"
#define BLYNK_AUTH_TOKEN "nZHDJx1UHvrR3iKXHy9vhVEJzRQ0GupX"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

#define DHTPIN 15          // DHT22 data pin
#define DHTTYPE DHT22     // DHT type
#define PIR_PIN 2        // PIR sensor pin
#define MOTION_LED_PIN 13 // LED pin for motion detection
#define TEMP_LED_PIN 12   // LED pin for temperature threshold
#define MOTION_INTERVAL 5000 // Motion check interval (milliseconds)

char auth[] = "nZHDJx1UHvrR3iKXHy9vhVEJzRQ0GupX"; // Blynk authentication token
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

int motionCount = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(PIR_PIN, INPUT);
  pinMode(MOTION_LED_PIN, OUTPUT);
  pinMode(TEMP_LED_PIN, OUTPUT);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(MOTION_INTERVAL, checkMotion);
}

void loop() {
  Blynk.run();
  timer.run();
}

void checkMotion() {
  int motionDetected = digitalRead(PIR_PIN);
  if (motionDetected == HIGH) {
    motionCount++;
    Blynk.virtualWrite(V1, motionCount); // Update motion count on Blynk
    blinkMotionLED(); // Blink LED for motion
    Serial.println("Motion is detected!");
  }
  
  float temperature = dht.readTemperature(); // Read temperature
  Blynk.virtualWrite(V2, temperature); // Update temperature on Blynk
  
  if (temperature >= 40.0) {
    blinkTempLED(); // Blink LED if temperature exceeds 40 degrees Celsius
    Serial.println("The temperature is beyond the threshold.");
  } else {
    digitalWrite(TEMP_LED_PIN, LOW); // Turn off LED if temperature is below 40 degrees Celsius
  }
}

void blinkMotionLED() {
  digitalWrite(MOTION_LED_PIN, HIGH); // Turn on motion LED
  delay(5000); // Wait for 500 milliseconds
  digitalWrite(MOTION_LED_PIN, LOW); // Turn off motion LED
}

void blinkTempLED() {
  digitalWrite(TEMP_LED_PIN, HIGH); // Turn on temperature LED
  delay(50000); // Wait for 500 milliseconds
  digitalWrite(TEMP_LED_PIN, LOW); // Turn off temperature LED
}
