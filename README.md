<div align="center">

# 🤖 ESP32 Analog Kalman Robot Arm
### Smooth Real-Time Servo Control using ESP32 + PCA9685 + Kalman Filter

<img src="https://img.shields.io/badge/ESP32-Embedded-red?style=for-the-badge&logo=espressif" />
<img src="https://img.shields.io/badge/PlatformIO-IoT-orange?style=for-the-badge&logo=platformio" />
<img src="https://img.shields.io/badge/PCA9685-Servo%20Driver-blue?style=for-the-badge" />
<img src="https://img.shields.io/badge/Kalman-Filter-green?style=for-the-badge" />

<br>

🎮 Potentiometer Controlled  
⚡ Smooth Servo Motion  
🧠 Kalman Filter Stabilization  
✋ Push Button Gripper Control  

</div>

---

# 📖 Overview

This project is a smooth and responsive **ESP32 Robot Arm Controller**
built with **PlatformIO** using:

- 🎮 Analog potentiometers for real-time control
- ⚡ PCA9685 servo driver for stable PWM generation
- 🧠 Kalman filter for motion smoothing
- ✋ Push-button gripper control

Perfect for:

- 🤖 DIY Robot Arm Projects
- 📚 Embedded Systems Learning
- 🔧 Servo Motion Experiments
- 🎓 Robotics Education

---

# ✨ Features

| Feature | Description |
|:--|:--|
| 🎮 Real-Time Control | Control robot joints using potentiometers |
| 🧠 Kalman Filtering | Smooth analog input and reduce jitter |
| ⚡ PCA9685 Driver | Stable multi-servo PWM control |
| 🔧 Expandable Design | Easy to customize and improve |
| 📟 Serial Monitoring | Real-time angle debugging |
| 🧹 Clean Code Structure | Beginner-friendly organization |

---

# 🛠 Hardware Used

| Component | Description |
|:--|:--|
| 🧠 ESP32 | Main microcontroller |
| ⚡ PCA9685 | 16-channel servo driver |
| 🔩 Servo Motors | Robot arm joints |
| 🎛 Potentiometers | Analog motion input |
| 🔘 Push Button | Gripper control |
| 🔋 External Power Supply | Recommended for servos |

---

# 🔌 Pin Configuration

## 📡 I2C Connection (PCA9685)

| ESP32 Pin | Function |
|:--|:--|
| GPIO 21 | SDA |
| GPIO 22 | SCL |

---

## 🎛 Potentiometers

| Joint | GPIO |
|:--|:--|
| Wrist | GPIO 32 |
| Elbow | GPIO 35 |
| Shoulder | GPIO 34 |
| Base | GPIO 39 |

---

## 🔘 Button Input

| Function | GPIO |
|:--|:--|
| Gripper Control | GPIO 33 |

---

## ⚡ PCA9685 Servo Channels

| Servo | Channel |
|:--|:--|
| Hand / Gripper | 11 |
| Wrist | 12 |
| Elbow | 13 |
| Shoulder | 14 |
| Base | 15 |

---

# 🧠 Kalman Filter Motion Smoothing

This project uses a lightweight Kalman Filter to smooth noisy analog readings.

## ✅ Benefits

- smoother servo movement
- reduced jitter
- stable positioning
- responsive control

---

## ⚙️ Filter Tuning

```cpp
KalmanFilter wristFilter = {0, 1, 25, 0.4};
