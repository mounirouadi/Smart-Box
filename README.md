# Arduino Smart Box
This Arduino project creates a smart box that uses a fingerprint sensor for authentication, an RTC module (RtcDS1302) to get the current time, and an SD card module to save logs with timestamps and display them on an I2C LCD screen.

# Hardware
Arduino board (e.g. Uno, Mega, etc.)
Fingerprint sensor
RTC module (RtcDS1302)
SD card module
I2C LCD screen
Jumper wires
Breadboard (optional)
# Software
Arduino IDE (download at https://www.arduino.cc/en/Main/Software)
Fingerprint sensor library (instructions for installation can be found in the sensor's datasheet)
RtcDS1302 library (available through the Arduino IDE's library manager or from the author's GitHub repository at https://github.com/jcw/rtclib)
SD card module library (e.g. SD library, available through the Arduino IDE's library manager)
I2C LCD library (e.g. LiquidCrystal_I2C library, available through the Arduino IDE's library manager)
# Setup
Connect the hardware according to the schematics in the schematics folder.
Install the required software libraries (see "Software" section above).
Open the smart_box.ino file in the Arduino IDE.
Modify the constants at the top of the file (e.g. FINGERPRINT_SENSOR_RX_PIN, FINGERPRINT_SENSOR_TX_PIN, LCD_I2C_ADDRESS, etc.) to match the connections made in step 1.
Upload the code to the Arduino board.
# Usage
Press the button connected to the Arduino to begin authentication.
Place your finger on the fingerprint sensor to authenticate. If the authentication is successful, the LCD will display a message and a log will be saved to the SD card with a timestamp.
Repeat step 2 to open and close the smart box.
# Customization
You can customize the behavior of the smart box by modifying the code in the smart_box.ino file. For example, you can change the messages displayed on the LCD, the number of allowed attempts before the authentication process is terminated, or the actions taken after a successful authentication.

Note: If you want to use a different I2C LCD library, make sure to modify the library and function calls in the code accordingly.
