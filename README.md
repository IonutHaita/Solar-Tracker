# Solar-Tracker
 A solar tracking project that I made using ESP32


Demo video: https://youtu.be/LBXWywKYlnA

Notes and observations:

- The device was not designed as a long-time solution, it was made as a proof of concept and also as a challenge that I set for myself;
- The 3D printed parts are not refined and will require slight adjustments(drilling holes, surface rectifications, etc.) to fit with each other;
- The code might require additional libraries in case you decide to use another hardware(development kit, display or motors);
- The code uses Serial Bluetooth library to send real time data from sensors and servo positions as they change. In order to use this feature you must install Bluetooth Terminal on your device(Smartphone, Laptop, PC, etc.) and pair the devices;
- Using other hardware is very easy as long as it has 5 ADC inputs and two servo(PWM) outputs. The BlueTooth function is optional and the respective code must be eliminated in case you decide not to use it. In case you want to use another hardware, you must check the ADC specs(max input signal voltage!). For ESP32 the max voltage is 3.3V and so I have connected the sensors to 3.3V and the max value is 4095(it can be changed in the code) so I used this value to map the range of the solar panel. In conclusion, if your hardware has other specs you will have to calculate the values of the resistors(R1->R5 in schematic)according to datasheet specs and change the value in the map function for the solar panel;
- Using other hardware will require the use of a debugging code to see if the new resistor value allow the sensors to work corectly. To check sensors and motors use the testing and debugging code that I wrote or write one that fits your needs;
