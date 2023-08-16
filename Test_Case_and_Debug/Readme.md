Important:

- Don't forget to change the pin reference before uploading code!
- Make sure you don't exceed the maximum input voltage on the ADC!
- Make sure your servos have separate power supply for your servos! Don't use the 5V pin on your dev kit to avoid damage to the linear voltage rectifier.

How this code works:

- Once the code has been uploaded to your dev kit open serial monitor, select baud rate 115200, select No Line Ending(or none) and send any character. You are now at main menu;
- Select your option as follows:
  1. Sensor values - single: This option will print the values from the LDRs once and is very useful if you want to set your tolerance;
  2. Sensor values - continous: This option will print the values from the LDRs in a loop until you send another character via serial monitor. It is used to check if the LDR pins are correctly assigned and if they are working properly; 
  3. Debug panel movement: With this option you can see in real time the direction of travel for the panel and for the servos.