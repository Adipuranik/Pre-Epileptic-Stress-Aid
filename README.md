#  Pre-Epileptic Stress Aid

An Arduino-based real-time monitoring system for early detection of epileptic stress indicators using temperature, humidity, and vibration sensors — with sub-second alert response via interrupt-driven architecture.

---

##  Hardware Used

| Component | Specification |
|-----------|--------------|
| Microcontroller | Arduino Uno (ATmega328P) |
| Temp & Humidity Sensor | DHT11 |
| Vibration Sensor | SW-420 (digital output) |
| Alert Output | Active Buzzer |
| Display | 16x2 LCD (I2C) |
| Power | 5V via USB |

---

##  Repository Structure

```
pre-epileptic-stress-aid/
├── src/
│   ├── stress_monitor.ino     # Main sketch with interrupt logic
│   ├── dht_handler.ino        # DHT11 sensor read functions
│   └── alert_system.ino       # Buzzer + LCD alert logic
├── hardware/
│   ├── circuit_diagram.md     # Wiring and pin connections
│   └── components_list.md     # Bill of materials
├── docs/
    └── working_principle.md   # Threshold logic and design decisions

```

---

##  How It Works

1. DHT11 continuously reads body-ambient **temperature and humidity**
2. SW-420 vibration sensor is wired to an **interrupt pin (INT0 / D2)** on the Arduino
3. Any vibration event triggers an **Interrupt Service Routine (ISR)** instantly — no polling delay
4. If vibration is detected AND temperature/humidity exceed defined thresholds simultaneously, the system flags a **stress event**
5. The buzzer fires and the LCD displays a warning message within milliseconds
6. ADC reads are optimized with controlled polling intervals to reduce false positives in motion-noisy environments

---

##  Alert Logic

```
Stress Event triggered when:
  vibration_detected == true (via INT0 interrupt)
  AND temperature > TEMP_THRESHOLD (default: 37.5°C)
  AND humidity > HUMID_THRESHOLD   (default: 70%)
```

Thresholds are `#define` constants — easy to tune for specific use cases.

---

##  Features

- **Interrupt-driven vibration detection** — ISR response, not polling
- **Multi-sensor fusion** — combines motion + temperature + humidity
- **Configurable thresholds** — easily tunable via `#define` constants
- **Power-aware polling** — DHT11 read interval optimized to reduce CPU load
- **LCD + Buzzer dual alert** — visual + audio notification
- **False positive reduction** — debounce logic in ISR

---

## 🔌 Pin Configuration

| Arduino Pin | Connected To |
|-------------|-------------|
| D2 (INT0)   | SW-420 Vibration Sensor (OUT) |
| D4          | DHT11 Data Pin |
| D5          | Buzzer (+) |
| A4 (SDA)    | LCD SDA (I2C) |
| A5 (SCL)    | LCD SCL (I2C) |
| 5V          | Sensor VCC |
| GND         | Common Ground |

---

##  Getting Started

### Prerequisites
- Arduino IDE 1.8.x or 2.x
- Libraries required:
  - `DHT sensor library` by Adafruit
  - `LiquidCrystal_I2C`

Install via Arduino IDE → Tools → Manage Libraries

### Upload Steps
```bash
git clone https://github.com/Adipuranik/pre-epileptic-stress-aid.git
```
1. Open `src/stress_monitor.ino` in Arduino IDE
2. Select board: **Arduino Uno**
3. Select your COM port
4. Upload and open Serial Monitor at **9600 baud** to view live readings

---

## 📊 Sample Serial Output

```
[OK]   Temp: 36.2°C | Humidity: 65% | Vibration: NONE
[OK]   Temp: 36.8°C | Humidity: 67% | Vibration: NONE
[WARN] Temp: 37.9°C | Humidity: 74% | Vibration: DETECTED
[ALERT] *** STRESS EVENT — CHECK PATIENT ***
```

---

##  Key Concepts Demonstrated

- GPIO interrupt handling (INT0 external interrupt)
- ADC-based analog sensor reading
- ISR design with debounce
- Multi-sensor threshold logic
- Power consumption optimization via controlled polling
- Real-time embedded C programming on ATmega328P

---

##  Disclaimer

This is an academic prototype for demonstration purposes only. It is **not a certified medical device** and should not be used for clinical diagnosis or patient monitoring.

---

##  License

MIT License — free to use, modify, and distribute with attribution.

---

##  Author

**Aditi S Puranik**  
B.E. Electronics & Communication Engineering  
Vidyavardhaka College of Engineering, Mysuru  
[LinkedIn](https://linkedin.com/in/aditi-s-puranik-598629391) · [GitHub](https://github.com/Adipuranik)
