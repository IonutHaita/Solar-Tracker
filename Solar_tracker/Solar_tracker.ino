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

const int ldrul = 32;//Upper left sensor  
const int ldrur = 33;//Upper right sensor
const int ldrll = 35;//Lower left sensor
const int ldrlr = 34;//Lower right sensor
const int panel = 39;//Panel "+" pin

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
  int ul = analogRead(ldrul); 
  int ur = analogRead(ldrur); 
  int ll = analogRead(ldrll); 
  int lr = analogRead(ldrlr); 
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
    sprintf(line, "Panel voltage: %d mV", valpanel);
    SerialBT.println(line);

    sprintf(line, "Upper left sensor: %d", ul);
    SerialBT.println(line);
    
    sprintf(line, "Upper right sensor: %d", ur);
    SerialBT.println(line);

    sprintf(line, "Lower left sensor: %d", ll);
    SerialBT.println(line);

    sprintf(line, "Lower right sensor: %d,", lr);
    SerialBT.println(line);
    
  }  
  const int time = 100; 

  const int tol = 800;//we set a tolerance to help us ignore the naturally scattered light.

  int avgup = (ul + ur) / 2; //calculating the aritmetic average of upper sensors 
  int avgdown = (ll + lr) / 2; //calculating the artimetic average of lower sensors
  int avgleft = (ul + ll) / 2; //calculating the aritmetic average of left side sensors
  int avgright = (ur + lr) / 2; //calculating the aritmetic average of right side sensors

  int vertdiff = avgup - avgdown;//calculating the vertical value difference between upper sensors and lower sensors to find out if we have more light on the top or bottom 
  
  if(vertdiff < 0)
    {
      vertdiff = 1 * vertdiff;//this is in case the result is negative
    }
  
  int horizdiff = avgleft - avgright;//calculating the horizontal value difference between left and right sides to find out if we have more light on the left side or the right side

  if(horizdiff < 0)
    {
      horizdiff = 1 * horizdiff;// this is in case the result is negative
    }


  if (vertdiff > tol)/*Once the vertical difference value is higher than the tolerance the program is going to compare the up value vs the down value and act accordingly.*/
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

  if (horizdiff > tol)  
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
      sprintf(line, "Horizontal servo position: %d", servoh);
      SerialBT.println(line); 
      horizontal.write(servoh);
    }
  

  delay(time);
} 
