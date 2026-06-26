# 🌻 Solar Flower — Auto Solar Pilot

**A sunflower-inspired, fully autonomous dual-axis solar tracking system built on the ESP32 — no app, no cloud, no Wi-Fi required.**

[![Platform](https://img.shields.io/badge/Platform-ESP32-blue)](#hardware-components-used)
[![Language](https://img.shields.io/badge/Firmware-Arduino%20C++-00979D)](#software-used)
[![License](https://img.shields.io/badge/License-MIT-green)](LICENSE)

---

## 📑 Table of Contents
- [Project Overview](#project-overview)
- [Problem Statement](#problem-statement)
- [Objectives](#objectives)
- [Features](#features)
- [Hardware Components Used](#hardware-components-used)
- [Software Used](#software-used)
- [Working Principle](#working-principle)
- [Project Architecture](#project-architecture)
- [Repository Structure](#repository-structure)
- [Getting Started](#getting-started)
- [Results and Conclusion](#results-and-conclusion)
- [Advantages and Limitations](#advantages-and-limitations)
- [Future Enhancements](#future-enhancements)
- [Screenshots](#screenshots)
- [Team Members](#team-members)
- [References](#references)
- [License](#license)

---

## Project Overview

**Solar Flower (Auto Solar Pilot)** is a dual-axis solar tracking system that mimics **heliotropism** — the natural ability of sunflowers to orient themselves toward the sun throughout the day. The system uses four **Light Dependent Resistors (LDRs)** to continuously sense the direction of strongest sunlight, and two **servo motors** controlled by an **ESP32 microcontroller** to physically reorient a photovoltaic panel along both the horizontal and vertical axes.

This is a **fully standalone, offline system** — it runs entirely on the ESP32 with no Wi-Fi, internet, or companion app needed. It was built as a low-cost, easily reproducible prototype suitable for educational and small-scale residential use.

## Problem Statement

Fixed solar panels are simple and inexpensive but lose significant efficiency because they cannot adjust to the sun's changing position throughout the day and across seasons. This results in suboptimal energy capture for most of the panel's operating hours. Motorized dual-axis trackers solve this but are often expensive or mechanically complex, limiting their use in educational and budget-constrained settings.

**Solar Flower addresses this gap** by building a dual-axis tracker entirely from low-cost, widely available components (LDRs, servos, ESP32), making solar tracking technology accessible for learning, prototyping, and small-scale deployment — with zero ongoing software/cloud dependency.

## Objectives

- Design and implement a **dual-axis solar tracking mechanism** (horizontal + vertical) that follows the sun's daily and seasonal movement.
- Use **low-cost, readily available components** — LDR sensors, servo motors, and an ESP32 — to keep the system affordable.
- Improve **energy harvesting efficiency** compared to a fixed-panel baseline.
- Build a **fully autonomous control algorithm** that requires no manual intervention and no network connection.
- **Experimentally validate** tracking accuracy, energy gain, and system reliability under real sunlight.
- Lay the groundwork for **future IoT-enabled monitoring**, kept entirely optional and separate from the core tracking logic.

## Features

- 🌞 **Closed-loop dual-axis tracking** using differential light-intensity comparison across 4 LDRs
- ⚙️ **Servo-driven panel orientation** with calibrated angular limits to prevent over-rotation
- 🔌 **Regulated power supply** (LM7806) delivering a stable 6V DC rail to logic and actuators
- 🛰️ **Fully offline operation** — no Wi-Fi, no app, no account, no credentials of any kind
- 🧮 **Sensor calibration and thresholding** to prevent jitter from minor light fluctuations
- 📈 **Field-tested performance** with measured gains over a fixed-panel baseline
- 🧩 **Modular design** — scalable up for larger installations or down for portable/educational kits

## Hardware Components Used

| Component | Purpose |
|---|---|
| **ESP32 Dev Board** | Main controller — reads sensors and drives servos |
| **LDR (Light Dependent Resistor) × 4** | Detects directional sunlight intensity at the 4 corners of the panel |
| **Servo Motor × 2** | Actuates horizontal and vertical panel rotation |
| **Photovoltaic (PV) Panel** | Converts captured sunlight into DC electrical output |
| **LM7806 Voltage Regulator** | Provides a steady 6V DC supply to the ESP32 and servos |
| **Rechargeable Battery (optional)** | Stores surplus energy for night-time / low-light operation |
| **Cross-frame mount + barriers** | Physically separates the 4 LDRs to create directional shadowing for accurate light comparison |

## Software Used

- **Arduino IDE** — firmware development environment
- **C / C++ (Arduino framework)** — firmware language
- **ESP32Servo** library — PWM servo control on ESP32

No cloud platform, app, or account is required to run this project.

## Working Principle

1. **Sensing:** Four LDRs mounted at the corners of a cross-shaped frame on the panel continuously sense ambient light intensity. Each LDR outputs an analog voltage that varies inversely with the light falling on it.
2. **Signal Acquisition:** The ESP32's ADC channels read all four LDR voltages in real time.
3. **Decision Logic:** A comparison algorithm evaluates the difference in light intensity between **opposite sensor pairs** (left–right for the horizontal axis, top–bottom for the vertical axis).
4. **Actuation:** Based on which side reads brighter, the ESP32 sends PWM control signals to the corresponding servo motor, incrementally rotating the panel toward the brighter direction until the readings are balanced (i.e., the panel is facing the sun directly).
5. **Calibration & Thresholding:** Offset correction compensates for LDR-to-LDR sensitivity differences, and a sensitivity threshold prevents jitter from minor light fluctuations (shadows, reflections).

## Project Architecture

```
                ┌────────────────────┐
   ☀ Sunlight   │   4 × LDR Sensors   │  (corner-mounted, cross-frame)
   ───────────► │  (Top/Bottom/L/R)   │
                └─────────┬──────────┘
                          │ Analog voltage (light intensity)
                          ▼
                ┌────────────────────┐
                │   ESP32 (ADC in)   │
                │  Comparison Logic  │
                │  (L-R, Top-Bottom) │
                └─────────┬──────────┘
                          │ PWM control signal
                          ▼
                ┌────────────────────┐
                │  Servo 1 (Horiz.)  │
                │  Servo 2 (Vert.)   │
                └─────────┬──────────┘
                          │ Mechanical rotation
                          ▼
                ┌────────────────────┐
                │   PV Solar Panel   │──► DC Output ──► LM7806 Regulator ──► 6V rail (ESP32 + servos)
                └────────────────────┘
```

**Flow summary:** LDRs sense → ESP32 compares pairs → servos rotate panel toward brighter side → panel stays perpendicular to sun → improved DC output → regulated power feeds the control loop itself, and surplus can charge a battery for night operation.

## Repository Structure

```
solar-flower/
├── firmware/
│   └── SolarFlower/
│       └── SolarFlower.ino          # Main ESP32 sketch — autonomous LDR tracking
├── docs/
│   └── Auto_Solar_Pilot_Report.pdf  # Full project report
├── media/
│   ├── screenshots/                 # Photos / demo footage (add your own)
│   └── circuit-diagrams/            # Wiring & block diagrams (add your own)
├── .gitignore
├── LICENSE
└── README.md
```

## Getting Started

### Prerequisites
- Arduino IDE with ESP32 board support installed
- `ESP32Servo` library (install via Library Manager)

### Steps
```bash
git clone https://github.com/<your-username>/solar-flower.git
cd solar-flower/firmware/SolarFlower
```
Open `SolarFlower.ino` in the Arduino IDE, select **Board: ESP32 Dev Module**, choose the correct COM port, and click **Upload**.

That's it — no accounts, no tokens, no Wi-Fi setup needed. Power it on near a light source and it will begin tracking automatically.

## Results and Conclusion

Field testing compared the dual-axis tracker against a fixed-panel baseline under real outdoor sunlight across daily and seasonal sun-position changes. Key findings:

- The system **consistently kept the panel close to perpendicular** to incoming sunlight throughout the test period.
- **Average power output was measurably higher** than the fixed-panel baseline.
- **Control overhead was low** — the ESP32 and servos consumed a small fraction of the energy gained from improved tracking, confirming a net positive energy benefit.
- Tracking response was **fast and stable**, with minimal mechanical jitter after threshold calibration.
- Performance was **best under clear-sky conditions** and degraded under cloud cover or diffuse light, where LDR readings become less directional.

**Conclusion:** The project validates that a simple, biomimetic, closed-loop control strategy can meaningfully improve solar energy capture at very low component cost, making it a practical and educational prototype with a clear path toward future IoT-enabled, large-scale tracking systems.

## Advantages and Limitations

**Advantages**
- Significant efficiency improvement over fixed panels
- Very low component cost, easy to source and replicate
- Fully autonomous — no manual repositioning, no internet dependency
- Modular and scalable (educational kit ↔ larger installation)
- No accounts, tokens, or cloud services required to operate

**Limitations**
- Accuracy drops under cloudy / diffuse lighting conditions
- Servos and joints experience mechanical wear from continuous movement
- Tracking electronics consume a (small) share of the harvested energy
- LDRs can misread under uneven lighting or reflections
- Long-term outdoor exposure requires proper sealing of electronics

## Future Enhancements

- **Hybrid tracking algorithms** combining LDR feedback with astronomical solar-position calculations (SPA) for reliability under cloud cover
- **PID or fuzzy-logic control** for smoother, more precise servo positioning
- **Optional IoT monitoring layer** (e.g., Blynk or ThingSpeak) added as a separate, opt-in sketch for users who want remote dashboards — kept fully decoupled from the core offline tracking logic
- **Weather-aware logic** to pause/adjust tracking during high wind or storms
- **Machine learning-based adaptive tracking** for variable cloud conditions
- **Weatherproof enclosure and corrosion-resistant frame** for extended outdoor durability
- **Battery management system** for more efficient night-time/low-light energy buffering

## Screenshots

> Add your own images/GIFs to `media/screenshots/` and reference them below.

| Hardware Setup | Wiring / Circuit | Tracking in Action |
|---|---|---|
| ![Hardware setup](media/screenshots/hardware-setup.png) | ![Circuit diagram](media/circuit-diagrams/wiring-diagram.png) | ![Tracking demo](media/screenshots/tracking-demo.gif) |

## Team Members

| Name | Department | Email |
|---|---|---|
| Rathina Kumar P | ECE, SRM Institute of Science and Technology | rathinap@srmist.edu.in |
| Janani Priya R | ECE, SRM Institute of Science and Technology | jr6358@srmist.edu.in |
| Arjun A G | ECE, SRM Institute of Science and Technology | aa9532@srmist.edu.in |
| Sai Dharshan K | ECE, SRM Institute of Science and Technology | sd9422@srmist.edu.in |

## References

Full reference list is available in the [project report](docs/Auto_Solar_Pilot_Report.pdf), including works on solar position algorithms, PID/fuzzy-logic solar trackers, and IoT-based tracking systems.

## License

This project is licensed under the **MIT License** — see [LICENSE](LICENSE) for details.
