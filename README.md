<p align="center">
	<img src="http://content.arduino.cc/brand/arduino-color.svg" width="50%" />
</p>


## Arduino Parking-Sensor With Clock
This is a source code of a parking sensor and Clock . For this project I used Arduino UNO, Buzzer, Ultrasonic Distance sensor, DS1307 RTC Clock Module and 16x2 LCD.

The LCD is prtinting time and date  in normal status and if car gets to parking mode (reverse gear) arduino get to ultrasonic mode and LCD print distance and graphic characters in this mode sensor detecting that some object is near it.

The buzzer is changing frequencies regarding the distance. That was done with map function.
