const int ldrul = 5;//replace value with the coresponding pin address for the upper left LDR
const int ldrur = 6;//replace value with the coresponding pin address for the upper right LDR
const int ldrll = 7;//replace value with the coresponding pin address for the lower left LDR
const int ldrlr = 8;//replace value with the coresponding pin address for the lower right LDR
const int panel = 9;//replace value with the coresponding pin address for the solar panel

int ulval = 3000;//analogRead(ldrul);//read value from the upper left sensor and pass it into ulval variable
int urval = 3000;//analogRead(ldrur);//read value from the upper right sensor and pass it into urval variable
int llval = analogRead(ldrll);//read value from the lower left sensor and pass it into llval variable
int lrval = analogRead(ldrlr);//read value from the upper left sensor and pass it into ulval variable
int pval =  analogRead(panel);//read value from the upper left sensor and pass it into ulval variable

void displaySensValSingle()
{
  Serial.print("Upper left value: ");
  Serial.println(ulval);
  Serial.print("Upper right value: ");
  Serial.println(urval);
  Serial.print("Lower left value: ");
  Serial.println(llval);
  Serial.print("Lower right value: ");
  Serial.println(lrval);
  Serial.println("Exiting to Main Menu...");
  delay(200);
  preMenu();
}

void displaySensValCont()
{
  while(!Serial.available())
  {
    Serial.print("Upper left value: ");
    Serial.println(ulval);
    Serial.print("Upper right value: ");
    Serial.println(urval);
    Serial.print("Lower left value: ");
    Serial.println(llval);
    Serial.print("Lower right value: ");
    Serial.println(lrval);
    delay(300);
  }
  Serial.parseInt();
  Serial.read();
  Serial.println("Exiting to Main Menu...");
  delay(200);
  preMenu();
}

void debugMovement()
{
  while(!Serial.available())
  {
    const int time = 100; //experiment with time delay and see how often the position is adjusted
    const int tol = 800; 

    String horizdir = "neutral";
    String vertidir = "neutral";

    int avgup = (ulval + urval) / 2; 
    int avgdown = (llval + lrval) / 2;
    int avgleft = (ulval + llval) / 2;
    int avgright = (urval + lrval) / 2;

    int servoh = 90;
    int servov = 90;

    int dvert = avgup - avgdown;
    if(dvert < 0)
    {
      dvert = -1 * dvert;
    }

    int doriz = avgleft - avgright;
    if(doriz < 0)
    {
      doriz = -1 * doriz;
    }

    

    if (dvert > tol)
      {
        if (avgup > avgdown)
          {
            vertidir = "The panel is moving up";
            servov ++;
            if(servov == 180)
            {
              servov = 180;
            }
          }
        else if (avgup < avgdown)
          {
            vertidir = "The panel is moving down";
            servov --;
            if(servov == 0)
            {
              servov = 0;
            }
          }
          Serial.println(vertidir);
          Serial.print("Vertical servo is at(degrees): ");
          Serial.println(servov);
      }

    if (doriz > tol)  
      {
        if (avgleft > avgright) 
          {
            horizdir = "The panel is moving left";
            servoh --;
            if(servoh == 0)
            {
              servoh = 0;
            } 
          }
        else if (avgleft < avgright)
          {
            horizdir = "The panel is moving right";
            servoh ++;
            if(servoh == 180)
            {
              servoh = 180;
            }
          }
        Serial.println(horizdir);
        Serial.print("Horizontal servo is at(degrees): ");
        Serial.println(servoh);
      }
    delay(time);
  }
  Serial.parseInt();
  Serial.read();
  Serial.println("Exiting to Main Menu...");
  delay(200);
  preMenu();
}

void checkPanelVoltage()
{
  while(!Serial.available())
  {
  int panelVal = analogread(panel);
  panelVal = map(panelVal, 0, 4095, 0, 5000);//Don't forget to change the values according to the specs of the ADC and the maximum output of the solar panel
  Serial.print("Panel voltage: ");
  Serial.print(panelVal);
  delay(400);//change for refresh rate
  }
  Serial.parseInt();
  Serial.read();
  Serial.println("Exiting to Main Menu...");
  delay(200);
  preMenu();
}

void preMenu()
{ 
  Serial.println("========== Main Menu ==========");
  Serial.println("Select an option:");
  Serial.println("1. Sensor values - single");
  Serial.println("2. Sensor values - continuos");
  Serial.println("3. Debug panel movement");
  Serial.println("4. Check solar panel voltage")
  Serial.println("==============================");
  mainMenu();
}

void mainMenu()
{
  while(1)
  {
    if(Serial.available())
    {
      int option = Serial.parseInt();
      Serial.read();
      switch(option)
      {
        case 1:
        displaySensValSingle();
        break;
        case 2:
        displaySensValCont();
        break;
        case 3:
        debugMovement();
        break;
        case 4:
        checkPanelVoltage();
        break;
        default:
        Serial.println("Invalid option, try again!");
        preMenu();
        break;
      }
    }
  }
}

void setup()
{
  Serial.begin(115200);//start serial communication
}

void loop()
{
  if(Serial.available())
  {
    Serial.parseInt();
    Serial.read();
    preMenu();
  }
}