#  Smart Plant Health Monitoring System

##  Overview
The **Smart Plant Health Monitoring System** is an IoT-based solution designed to monitor plant health using **image-based color detection** along with environmental sensing. The system captures plant images, analyzes color variations to determine plant health, and provides real-time feedback through a cloud dashboard.

This project combines **ESP32**, **ESP32-CAM**, **OpenCV**, **Flask**, and **Blynk IoT** to create an end-to-end smart monitoring platform.

---

##  Technologies Used
- **ESP32**
- **ESP32-CAM**
- **Python (OpenCV)**
- **Flask (Backend Server)**
- **Blynk IoT Platform**
- **DHT11 / DHT22 Sensor**
- **RGB LED (Visual Feedback)**

---

## System Architecture
1. **ESP32-CAM** captures plant images.
2. Images are sent to a **Flask server**.
3. **OpenCV** analyzes RGB values from the image to determine plant health.
4. The analysis result is sent to **Blynk IoT**.
5. **ESP32** reads the Blynk data and controls RGB LEDs accordingly.
6. Temperature and humidity values are displayed live on the Blynk dashboard.

---

##  Project Files Description

### `sketch_apr15a.ino`
- Main Arduino sketch for the ESP32 / ESP32-CAM
- Connects the device to:
  - WiFi
  - Flask server
  - Blynk IoT platform
- Handles:
  - Image capture
  - Data transmission to server
  - LED control based on analysis result

###  `plant_analysis.py`
- Python script running on the Flask server
- Uses **OpenCV** to:
  - Extract RGB values from captured images
  - Analyze dominant color patterns
- Determines plant condition such as:
  - Healthy
  - Under-nourished
  - Dead / unhealthy
- Sends the result back to Blynk

---

##  Plant Health Detection Logic
- **Green Dominance** → Healthy Plant (Green LED)
- **Yellow / Pale Shades** → Under-nourished Plant (Red LED)
- **Brown / Dark Shades** → Dead or Unhealthy Plant (Blue LED)

---

##  Environmental Monitoring
- Temperature and humidity data are collected using a **DHT sensor**
- Values are displayed live on the **Blynk dashboard**
- LED-based feedback provides quick visual indication of plant status

---

##  Blynk IoT Configuration

###  Product ID
Use the following **Blynk Product ID** in your project:

## 👤 Author
**Metla Srinath**  
Undergraduate B.Tech – IoT  

---

## 📄 License
This project is developed for **academic and educational purposes**.

