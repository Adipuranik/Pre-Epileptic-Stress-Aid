/*
 * Pre-Epileptic Stress Aid Monitor
 * Author: Aditi S Puranik
 * Description: Real-time multi-sensor monitoring system for early detection
 *              of epileptic stress indicators using DHT11, SW-420 vibration
 *              sensor, interrupt-driven architecture, and ADC optimization.
 */

#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ─── Sensor & Pin Definitions ─────────────────────────────────────────────────
#define DHT_PIN         4       // DHT11 data pin
#define DHT_TYPE        DHT11
#define VIBRATION_PIN   2       // INT0 — hardware interrupt pin
#define BUZZER_PIN      5

// ─── Alert Thresholds (tune as needed) ────────────────────────────────────────
#define TEMP_THRESHOLD    37.5  // °C — above normal body temp
#define HUMID_THRESHOLD   70.0  // %RH — elevated humidity

// ─── Polling Interval ─────────────────────────────────────────────────────────
#define DHT_READ_INTERVAL 2000  // ms — DHT11 min stable read interval
#define DEBOUNCE_DELAY    200   // ms — vibration ISR debounce

// ─── Object Instantiation ─────────────────────────────────────────────────────
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ─── Shared ISR Variables (volatile) ─────────────────────────────────────────
volatile bool vibrationDetected = false;
volatile unsigned long lastVibrationTime = 0;

// ─── State Variables ──────────────────────────────────────────────────────────
float temperature = 0.0;
float humidity    = 0.0;
unsigned long lastDHTRead = 0;
bool alertActive  = false;

// ─────────────────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(VIBRATION_PIN, INPUT);

  // Attach interrupt on INT0 (D2) — triggers on RISING edge
  attachInterrupt(digitalPinToInterrupt(VIBRATION_PIN), vibrationISR, RISING);

  dht.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Stress Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1500);
  lcd.clear();

  Serial.println("=== Pre-Epileptic Stress Aid ===");
  Serial.println("Monitoring started.\n");
}

// ─────────────────────────────────────────────────────────────────────────────
void loop() {
  unsigned long now = millis();

  // Read DHT11 at controlled intervals (avoids sensor overload & false reads)
  if (now - lastDHTRead >= DHT_READ_INTERVAL) {
    lastDHTRead = now;
    readDHT();
  }

  // Check stress condition
  if (vibrationDetected) {
    vibrationDetected = false;  // Clear ISR flag
    evaluateStress();
  }

  // Normal status display
  if (!alertActive) {
    displayStatus();
  }

  delay(100);  // Small yield — keeps loop responsive without busy-waiting
}

// ─── ISR — Vibration Detected ────────────────────────────────────────────────
// Keep ISR minimal: just set flag + debounce timestamp
void vibrationISR() {
  unsigned long now = millis();
  if (now - lastVibrationTime > DEBOUNCE_DELAY) {
    vibrationDetected = true;
    lastVibrationTime = now;
  }
}

// ─── DHT11 Read ───────────────────────────────────────────────────────────────
void readDHT() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t) && !isnan(h)) {
    temperature = t;
    humidity    = h;
  } else {
    Serial.println("[ERROR] DHT read failed. Retrying next interval.");
  }
}

// ─── Stress Evaluation ────────────────────────────────────────────────────────
void evaluateStress() {
  Serial.print("[WARN] Vibration detected | ");
  Serial.print("Temp: "); Serial.print(temperature);
  Serial.print("°C | Humidity: "); Serial.print(humidity);
  Serial.println("%");

  if (temperature > TEMP_THRESHOLD && humidity > HUMID_THRESHOLD) {
    triggerAlert();
  } else {
    Serial.println("[OK]  Thresholds not exceeded — no alert.");
  }
}

// ─── Alert System ────────────────────────────────────────────────────────────
void triggerAlert() {
  alertActive = true;
  Serial.println("[ALERT] *** STRESS EVENT DETECTED — CHECK PATIENT ***");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("!! STRESS EVENT !!");
  lcd.setCursor(0, 1);
  lcd.print("Check Patient Now");

  // Rapid buzzer pattern for attention
  for (int i = 0; i < 5; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(150);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }

  delay(3000);   // Hold alert display
  alertActive = false;
  lcd.clear();
}

// ─── Status Display ───────────────────────────────────────────────────────────
void displayStatus() {
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print((char)223);  // degree symbol
  lcd.print("C H:");
  lcd.print((int)humidity);
  lcd.print("%   ");

  lcd.setCursor(0, 1);
  lcd.print("Status: Normal  ");

  // Serial log
  Serial.print("[OK]   Temp: ");
  Serial.print(temperature);
  Serial.print("C | Humidity: ");
  Serial.print(humidity);
  Serial.println("% | Vibration: NONE");
}
