# Circuit Diagram & Wiring Guide — Pre-Epileptic Stress Aid

## Pin Connections

### Arduino Uno → DHT11 Sensor

```
Arduino Uno     DHT11
───────────     ─────
D4          --> DATA pin
5V          --> VCC
GND         --> GND
```
> Add a 10kΩ pull-up resistor between DATA and VCC for stable readings.

---

### Arduino Uno → SW-420 Vibration Sensor

```
Arduino Uno     SW-420
───────────     ──────
D2 (INT0)   --> OUT (digital output)
5V          --> VCC
GND         --> GND
```
> D2 is used specifically because it supports hardware interrupt INT0 on Arduino Uno.
> This gives sub-millisecond response vs polling on a regular digital pin.

---

### Arduino Uno → 16x2 LCD (I2C Module)

```
Arduino Uno     I2C LCD
───────────     ───────
A4 (SDA)    --> SDA
A5 (SCL)    --> SCL
5V          --> VCC
GND         --> GND
```

---

### Arduino Uno → Active Buzzer

```
Arduino Uno     Buzzer
───────────     ──────
D5          --> (+) Positive
GND         --> (-) Negative
```

---

## Full Wiring Diagram (Text)

```
                  ┌──────────────────────────┐
                  │       Arduino Uno         │
  USB/5V ───────► │ 5V               D2(INT0) │──── SW-420 OUT
                  │                  D4       │──── DHT11 DATA
                  │                  D5       │──── Buzzer(+)
                  │                  A4(SDA)  │──── LCD SDA
                  │                  A5(SCL)  │──── LCD SCL
                  │                  5V       │──── DHT11 VCC
                  │                  5V       │──── SW-420 VCC
                  │                  5V       │──── LCD VCC
                  │                  GND      │──── All GND
                  └──────────────────────────┘
```

---

## Important Notes

1. **INT0 is critical** — vibration sensor MUST be on D2 (or D3 for INT1) for interrupt to work
2. **DHT11 pull-up** — 10kΩ between DATA and 5V prevents floating reads
3. **Debounce** — SW-420 can trigger multiple times per event; firmware debounce (200ms) handles this
4. **Buzzer type** — use ACTIVE buzzer; passive buzzers need PWM tone() calls
