# 🫀 AI-Based Vital Signs Monitor
### STM32F411 · FreeRTOS · TensorFlow Lite · Custom PCB

> A real-time, multi-parameter vital signs monitoring system built from the ground up on an STM32F411 microcontroller — combining embedded firmware, real-time OS task scheduling, on-device AI inference, and custom PCB design into a single medical-grade embedded platform.

---

## 👨‍💻 About the Author

I'm an Electrical Engineer specializing in Computer and Embedded Systems Engineering. This project is my Final Year Project (FYP) and represents everything I'm passionate about — writing firmware that runs on real hardware, designing circuits that work in the real world, and pushing microcontrollers to their limits with AI at the edge.

---

## 📌 Project Overview

This system non-invasively monitors three critical vital signs **simultaneously and in real-time**:

| Parameter | Sensor | Interface |
|-----------|--------|-----------|
| SpO2 & Heart Rate | MAX30102 | I2C |
| Body Temperature | MLX90614 | I2C |
| ECG (Cardiac Activity) | ECG Module | ADC |

All parameters are displayed live on a **7-inch DWIN LCD touchscreen** with real-time waveforms and configurable threshold alerts for clinical readability.

---

## ⚙️ System Architecture

### Firmware — FreeRTOS Task Design

The entire firmware is built around **FreeRTOS**. Each sensor runs in its own dedicated task:

- `Task_ECG` — Continuous ADC sampling of cardiac signal
- `Task_SpO2` — MAX30102 SpO2 and heart rate acquisition
- `Task_Temp` — MLX90614 body temperature reading
- `Task_Display` — Real-time DWIN LCD update via UART

Tasks communicate via **FreeRTOS queues and semaphores**, ensuring non-blocking, concurrent data collection with precise timing across all channels. No sensor starves another.

### On-Device AI — TensorFlow Lite

A TensorFlow Lite model runs **entirely on the STM32F411** — no cloud, no external processor:

- Performs anomaly detection on incoming vital sign data
- Triggers early warning alerts for irregular readings
- Achieves **94%+ classification accuracy** on test data
- Operates within the tight memory and compute constraints of an ARM Cortex-M4

This demonstrates that meaningful AI inference is practical on resource-constrained embedded hardware.

### Hardware — Custom PCB

Designed a **custom 2-layer PCB** in EasyEDA featuring:

- Low-noise analog front-end for ECG and biosignal acquisition
- Proper power decoupling across all supply rails
- Signal conditioning for medical-grade signal integrity
- Clean separation of analog and digital ground planes

---

## 🛠️ Tech Stack

| Category | Details |
|----------|---------|
| Microcontroller | STM32F411CEU6 (Black Pill) — ARM Cortex-M4 @ 100MHz |
| RTOS | FreeRTOS |
| ML Framework | TensorFlow Lite for Microcontrollers |
| Sensors | MAX30102, MLX90614, ECG Module |
| Display | 7-inch DWIN LCD (UART) |
| PCB | Custom 2-layer, designed in EasyEDA |
| Languages | C, C++ |
| IDE | STM32CubeIDE |
| Protocols | I2C, UART, ADC |

---

## 📁 Repository Structure

```
├── AI based ECG/          # ECG signal processing and AI anomaly detection
├── Arduino Files/         # Sensor drivers and embedded firmware source files
├── DWIN HMI/             # DWIN LCD display interface and UI configuration
├── Result and Demo/       # Demo videos, output screenshots, and test results
└── README.md
```

---

## 🔌 Hardware Setup

### Pin Configuration (STM32F411 Black Pill)

| Peripheral | Pin(s) | Protocol |
|------------|--------|----------|
| MAX30102 | PB6 (SCL), PB7 (SDA) | I2C1 |
| MLX90614 | PB10 (SCL), PB11 (SDA) | I2C2 |
| ECG Module | PA0 | ADC1 |
| DWIN LCD | PA9 (TX), PA10 (RX) | UART1 |

---

## 🚀 Getting Started

### Prerequisites
- STM32CubeIDE
- STM32F411 Black Pill board
- MAX30102, MLX90614, ECG module, DWIN 7-inch LCD
- Custom PCB (Gerber files in `/PCB` folder) or breadboard wiring

### Build & Flash
1. Clone the repository
```bash
git clone https://github.com/yourusername/ai-vital-signs-monitor.git
```
2. Open the project in STM32CubeIDE
3. Build the project (`Ctrl + B`)
4. Flash to the STM32F411 via ST-Link

---

## 📸 Demo & Results

All pictures and Demo videos are in Resutls and Demo Folder

- Real-time SpO2, HR, Temperature, and ECG displayed simultaneously
- AI anomaly detection triggering threshold alerts on irregular readings
- 94%+ classification accuracy validated on test dataset




---

## 📄 License

Copyright © 2026 Muhammad Iqtada Ali. All rights reserved.

This project is shared for portfolio and demonstration purposes only. No part of this project may be used, copied, modified, or distributed without explicit written permission from the author.

For inquiries: iqtada.ali121@gmail.com

---

## 📬 Contact

**Muhammad Iqtada Ali**  
Electrical Engineer — Embedded Systems Specialist  
📧 iqtada.ali121@gmail.com  
🔗 [LinkedIn](https://linkedin.com/in/iqtada-ali-762b9637a)

> *If you found this project useful or want to collaborate, feel free to reach out.*
