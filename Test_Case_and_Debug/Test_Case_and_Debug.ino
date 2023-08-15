const int ldrul = 5;//replace value with the coresponding pin address for the upper left LDR
const int ldrur = 6;//replace value with the coresponding pin address for the upper right LDR
const int ldrll = 7;//replace value with the coresponding pin address for the lower left LDR
const int ldrlr = 8;//replace value with the coresponding pin address for the lower right LDR
const int panel = 9;//replace value with the coresponding pin address for the solar panel

int ulval = analogRead(ldrul);//read value from the upper left sensor and pass it into ulval variable
int urval = analogRead(ldrur);//read value from the upper right sensor and pass it into urval variable
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
          }
        else if (avgup < avgdown)
          {
            vertidir = "The panel is moving down";
          }
          Serial.println(vertidir);
      }

    if (doriz > tol)  
      {
        if (avgleft > avgright) 
          {
            horizdir = "The panel is moving left"; 
          }
        else if (avgleft < avgright)
          {
            horizdir = "The panel is moving right";
          }
        Serial.println(horizdir);
      }
  

    delay(time);
  }
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
  Serial.begin(9600);//start serial communication
}

void loop()
{
  preMenu();
}