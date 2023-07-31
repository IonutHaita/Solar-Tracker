#include <ESP32Servo.h>
#include <BluetoothSerial.h>
#include<LiquidCrystal_I2C.h>


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

LiquidCrystal_I2C lcd(0x27,16,2); //initializing the 16 characters by 2 lines LCD display at address 0x27(find out your display I2C address before uploading)

Servo horizontal; 
int servoh = 90; 

Servo vertical;  
int servov = 120;

int ldrul = 32;//Upper left sensor  
int ldrur = 33;//Upper right sensor
int ldrll = 35;//Lower left sensor
int ldrlr = 34;//Lower right sensor
int panel = 39;//Panel "+" pin

void setup()
{
   
  SerialBT.begin("Solar_Tracker_BT");

  lcd.init();
  lcd.backlight();

  horizontal.attach(18, 500, 2400); 
  vertical.attach(19, 500, 2400);
  horizontal.write(servoh);
  vertical.write(servov);
}

void loop() 
{
  int ul = analogRead(ldrss); 
  int ur = analogRead(ldrsd); 
  int ll = analogRead(ldrjs); 
  int lr = analogRead(ldrjd); 
  int valpanel = analogRead(panel);

  valpanel = map(valpanel, 0, 4095, 0, 5000);/*I used a panel that can output a maximum of 5V (5000mV) so I mapped the range of the ADC
 to match the maximum output of the panel. 5000 can be replaced by the maximum voltage output of your panel expressed in mV.*/ 

  char line[30];

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Panel voltage:");
  sprintf(line, "    %d mV", valpanel);
  lcd.setCursor(0,1);
  lcd.print(line);

  if (SerialBT.available()) 
  {
    sprintf(line, "Panel voltage: %d mV", valpanou);
    SerialBT.println(line);

    sprintf(line, "Upper left sensor: %d", ss);
    SerialBT.println(line);
    
    sprintf(line, "Upper right sensor: %d", sd);
    SerialBT.println(line);

    sprintf(line, "Lower left sensor: %d", js);
    SerialBT.println(line);

    sprintf(line, "Lower right sensor: %d,", jd);
    SerialBT.println(line);
    
  }  
  int time = 100; 

  int tol = 800;//we set a tolerance to help us ignore the naturally scattered light.

  int avgup = (ul + ur) / 2; //calculating the aritmetic average of upper sensors 
  int avgdown = (ll + lr) / 2; //calculating the artimetic average of lower sensors
  int avgleft = (ul + ll) / 2; //calculating the aritmetic average of left side sensors
  int avgright = (ur + lr) / 2; //calculating the aritmetic average of right side sensors

  int dvert = avgup - avgdown;//calculating the vertical value difference between upper sensors and lower sensors to find out if we have more light on the top or bottom 
  int doriz = avgleft - avgright;//calculating the horizontal value difference between left and right sides to find out if we have more light on the left side or the right side

  if (-1*dvert > tol || dvert > tol)/*the difference can be negative if avgup is a smaller number than avgdown so we multiply the tolerance with -1 and use the "OR" operand.
 Once the up or down value is higher than the tolerance the program is going to compare the up value vs the down value and act accordingly.*/
    {
      if (avgup > avgdown)//if the upper side value is higher than the lower side value we raise the panel
        {
          servov = ++servov;
          if (servov > 180)//if max limit reached, the servo stays there.
            {
              servov = 180;
            }
        }
      else if (avgup < avgdown)// if the lower side value is higher we lower the panel
        {
          servov= --servov;
          if (servov < 0)//if lower limit reached, the servo stays there
            {
              servov = 0; 
            }
        }
      sprintf(line, "Vertical servo position: %d", servov);
      SerialBT.println(line);  
      vertical.write(servov);
    }

  if (-1*doriz > tol || doriz > tol)  
    {
      if (avgleft > avgright) 
        {
          servoh = --servoh; 
          if (servoh < 0) 
            {
              servoh = 0;
            }
        }
      else if (avgleft < avgright)
        {
          servoh = ++servoh;
          if (servoh > 180)
          {
            servoh = 180;
          }
        }
      sprintf(linie, "Horizontal servo position: %d", servoo);
      SerialBT.println(linie); 
      orizontal.write(servoh);
    }
  

  delay(time);
} 
