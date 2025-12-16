  EnviroClock – Multifunction Alarm Clock with Room Temperature Display

EnviroClock is an embedded systems project based on the LPC2148 ARM7 microcontroller that integrates real-time clock functionality, room temperature monitoring, and alarm features into a single user-friendly device. The system displays time, date, day, alarm status, and room temperature on an LCD, with user interaction through a keypad and external interrupts.

---

  Project Objective

The objective of this project is to design and implement a multifunction digital clock that:
- Displays real-time clock information
- Monitors and displays room temperature
- Allows users to set and edit time and alarm
- Triggers an alarm using a buzzer at a predefined time

This project demonstrates practical usage of RTC, ADC, external interrupts, keypad interfacing, and LCD control in embedded C.

---

 Features

- Real-Time Clock (RTC) display (Time, Date, Day)
- Room temperature measurement using LM35
- Alarm setting with buzzer alert
- Keypad-based user interface
- External interrupt–based menu navigation
- Input validation for time and date parameters
- Automatic alarm timeout (1 minute)
- Modular and structured embedded C code

---

  Hardware Requirements

- LPC2148 Microcontroller
- 16x2 LCD Display
- Keypad
- LM35 Temperature Sensor
- Buzzer
- LEDs
- Push Buttons / Switches
- Power Supply and supporting passive components

---

  Software Requirements

- Embedded C
- Keil µVision IDE
- Flash Magic (for programming LPC2148)
- Proteus (optional, for simulation)

---

  Project Workflow

1. Initialize all peripherals:
   - LCD
   - Keypad
   - RTC
   - ADC (LM35)
   - LEDs
   - External Interrupts

2. Enter the main super loop:
   - Read time and date from RTC
   - Read temperature from LM35 via ADC
   - Display time, date, day, and temperature on LCD

3. Alarm Handling:
   - Continuously compare current time with alarm time
   - Activate buzzer when alarm time matches
   - Stop alarm using button or automatically after 1 minute

4. Interrupt-Based Menu System:
   - Triggered using an external interrupt
   - Menu options:
     - Edit RTC Information
     - Set Alarm
     - Exit

5. RTC Editing:
   - Modify hours, minutes, seconds, date, month, year
   - Input validation for all fields
   - Update RTC registers upon valid input

6. Alarm Setting:
   - User inputs alarm time (HH:MM)
   - Validated and stored for alarm comparison

---

 📂 Project Structure
“Enviroclock_Multifunction_Alarm_Clock_Mini Project ARM”
|   ├── src/
│   ├── main.c
│   ├── lcd.c
│   ├── adc.c
│   ├── rtc.c
│   ├── keypad.c
│   ├── interrupt.c
│
├── inc/
│   ├── lcd.h
│   ├── adc.h
│   ├── rtc.h
│   ├── keypad.h
│   ├── interrupt.h
│   ├── defines.h
│
├── proteus/
│   └── EnviroClock.pdsprj
│
├── keil_project/
│   └── project.uvproj
│
└── README.md
