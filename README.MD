# 💧 Automatic Water Level Controller

### ESP8266 (NodeMCU) + Tasmota Based Smart Pump Automation System

A fully automated, WiFi-enabled **Smart Water Tank Management System**
built using **NodeMCU (ESP8266)** and **Tasmota firmware**.\
This system intelligently monitors tank levels, protects the pump from
dry-run damage, prevents overflow, and provides a powerful web-based
dashboard for real-time monitoring and scheduling.

------------------------------------------------------------------------

## 🚀 Project Overview

This project automates water pump control using 8 discrete level sensors
and a flow sensor.\
It provides real-time monitoring through:

-   128x64 LCD display\
-   WS2812 LED water level strip\
-   Local web dashboard\
-   Calendar-based scheduling

Designed for residential buildings, apartments, farms, and smart homes.

------------------------------------------------------------------------

## 🧠 System Architecture

### 🔹 Controller Unit

-   NodeMCU (ESP8266 based)
-   Tasmota Firmware
-   8 Water Level Sensors
-   Flow Sensor
-   128x64 I2C LCD
-   8-bit WS2812 LED Strip

### 🔹 Pump Control Unit

-   Tasmota-based Smart Plug (Relay based)

------------------------------------------------------------------------

## ✨ Features

### 🌊 Smart Water Level Monitoring

-   8-point tank level detection
-   Real-time percentage display
-   LED visual indication (color coded)
-   LCD status updates

### 🛡️ Tank Full Protection

-   Automatically stops pump at maximum level
-   Prevents overflow and water wastage

### 🚱 Pump Dry Run Protection

-   Flow sensor validation
-   If no water flow detected → Pump shuts down
-   Protects motor from overheating

### 📅 Calendar-Based Scheduling

-   Time-based pump operation
-   Custom daily scheduling
-   Ideal for borewell or municipal water timing

### 📶 Automatic WiFi Provisioning

-   Captive portal on first boot
-   Easy WiFi configuration
-   No hardcoded credentials

### 🌐 Web-Based User Interface

Accessible via browser:

    http://device-ip

Includes 4 Custom Dashboards: - 📊 Real-Time Monitoring - ⚙️ Pump
Control - 📅 Schedule Management - 📈 Flow Analytics

Fully Local Control --- No Cloud Dependency.

------------------------------------------------------------------------

## 🔧 Hardware Requirements

### Controller Side

-   NodeMCU (ESP8266)
-   8 Water Level Sensors (Float / Conductive)
-   Flow Sensor
-   128x64 I2C LCD
-   WS2812 LED Strip (8 LEDs)
-   5V Power Supply

### Pump Control

-   Tasmota-compatible Smart Plug

------------------------------------------------------------------------

## ⚙️ Working Logic

1.  System continuously monitors tank levels.
2.  If level drops below threshold → Pump turns ON.
3.  Flow sensor validates water movement.
4.  If no flow detected → Dry run protection activates.
5.  When tank becomes full → Pump turns OFF automatically.
6.  Scheduled automation overrides manual control if enabled.

------------------------------------------------------------------------

## 🔐 Safety Mechanisms

-   Overflow protection
-   Dry run detection
-   Flow verification logic
-   Manual override support
-   Fail-safe reboot recovery

------------------------------------------------------------------------

## 🔗 Integration Support

-   MQTT Compatible
-   HTTP API Support
-   Works with Home Automation Platforms
-   Can integrate with Home Assistant, OpenHAB, Node-RED

------------------------------------------------------------------------

## 📊 Advantages

✔ Fully Local Control\
✔ Low-Cost Hardware\
✔ Energy Efficient\
✔ Expandable Architecture\
✔ Robust Safety Design

------------------------------------------------------------------------

## 🛠️ Future Enhancements

-   Mobile App Integration
-   SMS Alerts
-   Cloud Logging
-   Historical Data Analytics
-   AI-Based Water Usage Prediction

------------------------------------------------------------------------

## 📜 License

This project is open-source.\
Contributions, feature requests, and pull requests are welcome.

------------------------------------------------------------------------

## 👨‍💻 Author : aakash-pro

Developed with ❤️ for smart and efficient water management.
