
# Smart Metal Detection System using ESP32, MQTT & Adafruit IO

A cloud‑connected smart security solution that detects metals, automates access control, and streams real-time data to the cloud.

---

## **Overview**

This project is a **Smart Metal Detection System** designed for **security checkpoints and automated screening systems**.
It detects the presence of metal in bags, controls access barriers, gives visual & audio alerts, counts bags, and sends data to a **cloud dashboard (Adafruit IO)** for remote monitoring and control.

---

## **Features**

1. **Metal Detection** – Detects presence of metal using metal sensors.
2. **Access Control** – Automatically opens/closes barriers or doors using relays.
3. **Indications** –

   * Green LED → Safe
   * Red LED + Buzzer → Metal Detected
4. **Bag Counter** – IR sensors count the number of bags scanned.
5. **Cloud Integration** – Sends data to **Adafruit IO dashboard via MQTT** for real-time monitoring.

---

## **Tech Stack**

* **Hardware:** ESP32, Metal Sensor, IR Sensors, Relays, LEDs, Buzzer
* **Software/Cloud:**

  * Arduino IDE
  * MQTT Protocol
  * Adafruit IO (Cloud dashboard)

---

## **System Architecture**

```
[ Metal Sensor + IR Sensor ] 
           |
        [ ESP32 ]
           |
   -----------------
   |       |       |
 [LEDs] [Relay] [Buzzer]
           |
     MQTT Protocol
           |
     Adafruit IO Cloud
```

---

## **Components Used**

* ESP32 microcontroller
* Metal detection sensor
* IR sensors (for bag counting)
* Relay modules (for barrier/door control)
* LEDs (Red/Green indicators)
* Buzzer
* Power supply

---

## **Setup & How to Run**

1. Clone this repository:

   ```bash
   git clone https://github.com/mystery1234/smartmetalDetectionSystem
   ```
2. Open the code in **Arduino IDE**.
3. Install necessary libraries:

   * WiFi
   * Adafruit MQTT
4. Update your **WiFi credentials** and **Adafruit IO username/key** in the code.
5. Upload the code to ESP32.
6. Power up the system and check the **Adafruit IO dashboard** for live data!

---

## **Future Enhancements**

* Integration with **CCTV for automated recording**
* Data analytics on cloud (alerts, history tracking)
* Mobile app notifications

---

## **License**

This project is open-source under the MIT License.

---

 *Bringing IoT & automation together for smarter security.*

---
## Acknowledgment
This project was developed as part of a team during a hackathon.  
This repository has been cleaned, structured, and documented by Shaik Ruksana for learning and portfolio purposes.


