# Firmware

This folder contains the ESP32 firmware for Solar Flower — a fully **standalone**,
**offline** dual-axis solar tracker. No Wi-Fi, no app, no cloud account, no
tokens required.

## Files
- `SolarFlower.ino` — Main sketch. Reads 4 LDR sensors, compares opposite
  pairs (top/bottom and left/right), and drives two servos to keep the
  panel facing the brightest light source.

## How it works (quick version)
1. Four LDRs sit at the four corners of a cross-shaped frame on the panel.
2. The ESP32 reads each LDR's brightness value.
3. It compares **Top vs Bottom** → moves the vertical servo toward
   whichever side is brighter.
4. It compares **Left vs Right** → moves the horizontal servo toward
   whichever side is brighter.
5. A small threshold stops the panel from jittering over tiny light changes
   (clouds passing, shadows, reflections).
6. This repeats continuously, so the panel slowly re-centers on the sun
   throughout the day.

## Setup
1. Install the **Arduino IDE** and add ESP32 board support
   (File → Preferences → Additional Board URLs, then install "esp32" via
   Boards Manager — search "ESP32 Arduino").
2. Install the **ESP32Servo** library via Library Manager
   (Sketch → Include Library → Manage Libraries → search "ESP32Servo").
3. Open `SolarFlower.ino`.
4. Select **Board: ESP32 Dev Module**, pick the correct COM port.
5. Click Upload.

## Wiring

| ESP32 Pin | Connected To |
|-----------|--------------|
| GPIO 34 | LDR — Top |
| GPIO 35 | LDR — Bottom |
| GPIO 32 | LDR — Left |
| GPIO 33 | LDR — Right |
| GPIO 18 | Servo 1 (Horizontal axis) signal wire |
| GPIO 19 | Servo 2 (Vertical axis) signal wire |
| 5V/6V   | Servo power (via LM7806 regulator) |
| GND     | Common ground for everything |

Each LDR should be wired as a voltage divider: one leg to 3.3V, the other
leg to GND through a fixed resistor (commonly 10kΩ), with the ESP32 ADC
pin connected at the midpoint between the LDR and the resistor.

## Tuning tips
If the panel jitters or overshoots, open `SolarFlower.ino` and adjust:
- `LIGHT_THRESHOLD` — raise this if it's twitching on small light changes
- `STEP_DEGREES` — lower this for smoother/slower movement
- `LOOP_DELAY_MS` — raise this to slow down how often it re-checks

## Why no Wi-Fi / cloud app?
This version is intentionally self-contained — it works anywhere, needs no
account, no internet connection, and has no credentials that could ever be
accidentally exposed if you share this code publicly on GitHub. If you want
to add remote monitoring later (e.g., logging servo angles or sensor values
to the cloud), that can be added as a separate, optional sketch — just be
sure to keep any Wi-Fi password or API token out of the main `.ino` file
(see how `secrets.h` + `.gitignore` was used in earlier project iterations).
