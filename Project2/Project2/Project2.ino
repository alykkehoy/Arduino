// include the library code:
#include <LiquidCrystal.h>

const int CONTRAST_PIN = 6;
int contrastValue = 50;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int BUTTON1_PIN = 9;
const int BUTTON2_PIN = 10;
const int BUTTON3_PIN = 13;

int b1State;
int b2State;
int b3State;

int calcBase = 2;


void setup() {
  pinMode(CONTRAST_PIN, OUTPUT);
  analogWrite(CONTRAST_PIN, contrastValue); 
  lcd.begin(16, 2);
  lcd.noAutoscroll();

  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  pinMode(BUTTON3_PIN, INPUT);
  Serial.begin(9600);
}

void waitButton2(){
  b2State = digitalRead(BUTTON2_PIN);
  while(b2State == LOW){
    b2State = digitalRead(BUTTON2_PIN);
  }
  return;  
}

//Display the intro splash screen
void splashScreen(){
  lcd.clear();
  lcd.print("Welcome");
  lcd.setCursor(0, 1);
  lcd.print("User!");

  waitButton2();
  return;
}

//get a byte number
int getNum(int num){
  lcd.clear();
  lcd.print("Number " + String(num) + ":");
  Serial.print("Number " + String(num) + ":\n");

  int printNum = 0;
  //for each of the 8 bit positions get 1 or 0
  for(int i = 0; i < 8; i++){
    b1State = digitalRead(BUTTON1_PIN);
    b2State = digitalRead(BUTTON2_PIN);

    bool state = true;
    bool button1Up = true;

    //wait for the submit button to come back up
    while(b2State != LOW){
      b2State = digitalRead(BUTTON2_PIN);
      delay(100);
    }

    lcd.setCursor(i,1);
    lcd.print("0");

    //while the submit button is not pressed check for the other button to swap the current bit
    while(b2State == LOW){
      b1State = digitalRead(BUTTON1_PIN);
      b2State = digitalRead(BUTTON2_PIN);

      if(b1State == LOW){
        button1Up = true;
      }

      //if the other button is pressed flip the current bit
      if(button1Up == true && b1State != LOW){
        state = !state;
        button1Up = false;
        lcd.setCursor(i,1);
        if(state){
          lcd.print("0");
          Serial.print(0);
        }else{
          lcd.print("1");
          Serial.print(1);
        }
      }
    }
    //if the current bit state is set, set the bit in the printnum
    if(!state){
      bitSet(printNum, 8 - (i + 1));
      Serial.println(printNum);
    }
    Serial.print("\nNext\n");
  }
  Serial.println(printNum);
  delay(2000);
  return printNum;
}

//Screen for the calculator
void calcSum(){
  //get the 2 nubmers to be added
  int num1 = getNum(1);
  int num2 = getNum(2);

  int sum = num1 + num2;
  lcd.clear();
  lcd.print("Sum is:");
  lcd.setCursor(0,1);
  lcd.print(arbitraryBase(sum, calcBase));


  delay(1000);
  int tempBase = calcBase;
  bool b1Pressed = false;
  bool b3Pressed = false;

  b2State = digitalRead(BUTTON2_PIN);  
  while(b2State == LOW){
    b1State = digitalRead(BUTTON1_PIN);
    b2State = digitalRead(BUTTON2_PIN);
    b3State = digitalRead(BUTTON3_PIN);

    if(b3State != LOW && !b3Pressed){
      if(tempBase < 16){
        b3Pressed = true;
        tempBase++;
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print(arbitraryBase(sum, tempBase));
      }
    }
    if(b3State == LOW){
      b3Pressed = false;
    }
    
    if(b1State != LOW && !b1Pressed){
      if(tempBase > 2){
        b1Pressed = true;
        tempBase--;
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print(arbitraryBase(sum, tempBase));
      }
    }
    if(b1State == LOW){
      b1Pressed = false;
    }
  }
}

void changeBase(){
  int base = 2;
  bool b1Pressed = false;
  bool b3Pressed = false;

  delay(1000);
  
  lcd.clear();
  lcd.print("New Base: ");
  lcd.setCursor(0,1);
  lcd.print(base);

  b2State = digitalRead(BUTTON2_PIN);
  while(b2State == LOW){
    b1State = digitalRead(BUTTON1_PIN);
    b2State = digitalRead(BUTTON2_PIN);
    b3State = digitalRead(BUTTON3_PIN);
    
    if(b1State != LOW && !b1Pressed){
      if(base == 10){
        lcd.setCursor(1,1);
        lcd.print(" ");
      }
      if(base > 2){
        b1Pressed = true;
        base--;
        lcd.setCursor(0,1);
        lcd.print(base);
      }
    }
    if(b1State == LOW){
      b1Pressed = false;
    }

    if(b3State != LOW && !b3Pressed){
      if(base < 16){
        b3Pressed = true;
        base++;
        lcd.setCursor(0,1);
        lcd.print(base);
      }
    }
    if(b3State == LOW){
      b3Pressed = false;
    }
  }

  int num = getNum(1);
  String newNumString = arbitraryBase(num, base);
  
  lcd.clear();
  lcd.print("In base " + String(base) + ":");
  lcd.setCursor(0,1);
  lcd.print(newNumString);
  delay(1000);
  waitButton2();
}

void setCalcBase(){
  bool b1Pressed = false;
  bool b3Pressed = false;

  delay(1000);
  
  lcd.clear();
  lcd.print("New Calc Base: ");
  lcd.setCursor(0,1);
  lcd.print(calcBase);

  b2State = digitalRead(BUTTON2_PIN);
  while(b2State == LOW){
    b1State = digitalRead(BUTTON1_PIN);
    b2State = digitalRead(BUTTON2_PIN);
    b3State = digitalRead(BUTTON3_PIN);
    
    if(b1State != LOW && !b1Pressed){
      if(calcBase == 10){
        lcd.setCursor(1,1);
        lcd.print(" ");
      }
      if(calcBase > 2){
        b1Pressed = true;
        calcBase--;
        lcd.setCursor(0,1);
        lcd.print(calcBase);
      }
    }
    if(b1State == LOW){
      b1Pressed = false;
    }

    if(b3State != LOW && !b3Pressed){
      if(calcBase < 16){
        b3Pressed = true;
        calcBase++;
        lcd.setCursor(0,1);
        lcd.print(calcBase);
      }
    }
    if(b3State == LOW){
      b3Pressed = false;
    }
  }
}

String arbitraryBase( int value, int base) {
   static char baseChars[] = "0123456789ABCDEF";
   String result = "";
   do {
        result = String(baseChars[value % base]) + result;
        value /= base;
       } while (value != 0);
   return result;
}

//Display the contrast screen
void contrastScreen(){
  delay(1000);
  lcd.clear();
  lcd.print("Contrast: ");
  lcd.setCursor(0,1);
  lcd.print(contrastValue);

  bool b1Pressed = false;
  bool b3Pressed = false;

  b2State = digitalRead(BUTTON2_PIN);  
  while(b2State == LOW){
    b1State = digitalRead(BUTTON1_PIN);
    b2State = digitalRead(BUTTON2_PIN);
    b3State = digitalRead(BUTTON3_PIN);

    //if the right button is pressed increase the contrast
    if(b3State != LOW && !b3Pressed){
      b3Pressed = true;
      contrastValue = contrastValue + 5;
      lcd.setCursor(0,1);
      lcd.print(contrastValue);
    }
    //reset button 1 pressed boolean
    if(b3State == LOW){
      b3Pressed = false;
    }
    
    //if the left button is pressed decrease the contrast
    if(b1State != LOW && !b1Pressed){
      if(contrastValue == 10){
        lcd.setCursor(1,1);
        lcd.print(" ");
      }
      b1Pressed = true;
      contrastValue = contrastValue - 5;
      lcd.setCursor(0,1);
      lcd.print(contrastValue);
    }
    //reset button 2 pressed boolean
    if(b1State == LOW){
      b1Pressed = false;
    }

    //write the new contrast value
    analogWrite(CONTRAST_PIN, contrastValue); 
  }
}

void mainMenu(){
  delay(1000);
  int menuScreen = 0;
  bool b1Pressed = false;
  bool b3Pressed = false;

  updateMenuScreen(menuScreen);

  b2State = digitalRead(BUTTON2_PIN);  
  while(b2State == LOW){
    b1State = digitalRead(BUTTON1_PIN);
    b2State = digitalRead(BUTTON2_PIN);
    b3State = digitalRead(BUTTON3_PIN);

    if(b3State != LOW && !b3Pressed){
      b3Pressed = true;
      menuScreen++;
      updateMenuScreen(menuScreen);
    }
    if(b3State == LOW){
      b3Pressed = false;
    }
    
    if(b1State != LOW && !b1Pressed){
      if(menuScreen > 0){
        b1Pressed = true;
        menuScreen--;
        updateMenuScreen(menuScreen);
      }
    }
    if(b1State == LOW){
      b1Pressed = false;
    }
  }
  
  gotoMenuScreen(menuScreen);
}

void updateMenuScreen(int screen){
  lcd.clear();
  switch(screen){
    case 0:
      lcd.print("   Calculator  >");
      lcd.setCursor(0,1);
      lcd.print("     Menu      >");
      break;
    case 1:
      lcd.print("<   Addition   >");
      lcd.setCursor(0,1);
      lcd.print("<              >");
      break;
    case 2:
      lcd.print("<  Subtraction >");
      lcd.setCursor(0,1);
      lcd.print("<              >");
      break;
    case 3:
      lcd.print("<Multiplication>");
      lcd.setCursor(0,1);
      lcd.print("<              >");
      break;
    case 4:
      lcd.print("<    Change    >");
      lcd.setCursor(0,1);
      lcd.print("<     Base     >");
      break;
    case 5:
      lcd.print("<Set Calculator>");
      lcd.setCursor(0,1);
      lcd.print("<     Base     >");
      break;
    case 6:
      lcd.print("<    Adjust     ");
      lcd.setCursor(0,1);
      lcd.print("<   Contrast    ");
      break;
  }
}

void gotoMenuScreen(int screen){
  switch(screen){
    case 0:
//      lcd.print("welcome test");
      break;
    case 1:
      calcSum();
      break;
    case 4:
      changeBase();
      break;
    case 5:
      setCalcBase();
      break;
    case 6:
      contrastScreen();
      break;
  }
}

void loop() {
  splashScreen();
//  calcSum();
//  changeBase();
//  contrastScreen();
  mainMenu();
}
