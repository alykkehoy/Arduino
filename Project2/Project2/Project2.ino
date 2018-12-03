// include the library code:
#include <LiquidCrystal.h>

static char baseChars[] = "0123456789ABCDEF";

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

int getNum(int num){
  lcd.clear();
  lcd.print("Num " + String(num) + ":");
  lcd.setCursor(8,0);
  for(int i = 0; i < 8; i++){
    lcd.print("0");
  }
  lcd.setCursor(8, 1);
  lcd.print("^");

  delay(1000);
  
  int returnNum = 0;
  int inputNums[] = {0,0,0,0,0,0,0,0};
  int curLoc = 0;
  bool b1Pressed = false;
  bool b2Pressed = false;
  bool b3Pressed = false;

  while(curLoc < 8){
    b1State = digitalRead(BUTTON1_PIN);
    b2State = digitalRead(BUTTON2_PIN);  
    b3State = digitalRead(BUTTON3_PIN);  

    if(b1State != LOW && !b1Pressed){
      if(curLoc > 0){
        lcd.setCursor(curLoc + 8, 1);
        lcd.print(" ");
        b1Pressed = true;
        curLoc--;
        lcd.setCursor(curLoc + 8, 1);
        lcd.print("^");
      }
    }
    if(b1State == LOW){
      b1Pressed = false;
    }

    if(b3State != LOW && !b3Pressed){
      lcd.setCursor(curLoc + 8, 1);
      lcd.print(" ");
      b3Pressed = true;
      curLoc++;
      lcd.setCursor(curLoc + 8, 1);
      lcd.print("^");
    }
    if(b3State == LOW){
      b3Pressed = false;
    }

    if(b2State != LOW && !b2Pressed){
      b2Pressed = true;
      inputNums[curLoc]++;
      lcd.setCursor(curLoc + 8, 0);
      lcd.print(baseChars[inputNums[curLoc] % calcBase]);
    }
    if(b2State == LOW){
      b2Pressed = false;
    }
  }

  for(int i = 0; i < 8; i++){
    Serial.println(inputNums[7 - i] % calcBase);
    Serial.println(round(pow(calcBase, i)));
    returnNum += ((inputNums[7 - i] % calcBase) * round(pow(calcBase, i)));
  }
  Serial.println(returnNum);
  return returnNum;
}

//Screen for the calculator
void calcSum(){
  //get the 2 nubmers to be added
  int num1 = getNum(1);
  int num2 = getNum(2);

  int sum = num1 + num2;
  lcd.clear();
  lcd.print("Sum:");
  lcd.setCursor(14,0);
  lcd.print(calcBase);
  lcd.setCursor(0,1);
  lcd.print(arbitraryBase(sum, calcBase));

  displayResult(sum);
}

void calcDiff(){
  //get the 2 nubmers to be added
  int num1 = getNum(1);
  int num2 = getNum(2);

  int difference = num1 - num2;
  lcd.clear();
  lcd.print("Difference:");
  lcd.setCursor(14,0);
  lcd.print(calcBase);
  lcd.setCursor(0,1);
  lcd.print(arbitraryBase(difference, calcBase));

  displayResult(difference);
}

void calcProd(){
  //get the 2 nubmers to be added
  int num1 = getNum(1);
  int num2 = getNum(2);

  int prod = num1 * num2;
  lcd.clear();
  lcd.print("Product:");
  lcd.setCursor(14,0);
  lcd.print(calcBase);
  lcd.setCursor(0,1);
  lcd.print(arbitraryBase(prod, calcBase));

  displayResult(prod);
}

void displayResult(int result){
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
        lcd.setCursor(14,0);
        lcd.print(tempBase);
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print(arbitraryBase(result, tempBase));
      }
    }
    if(b3State == LOW){
      b3Pressed = false;
    }
    
    if(b1State != LOW && !b1Pressed){
      if(tempBase > 2){
        if(tempBase == 10){
          lcd.setCursor(15,0);
          lcd.print(" ");
        }
        b1Pressed = true;
        tempBase--;
        lcd.setCursor(14,0);
        lcd.print(tempBase);
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print(arbitraryBase(result, tempBase));
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
      lcd.print("      Menu     >");
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
//      getNum(1);
      break;
    case 1:
      calcSum();
      break;
    case 2:
      calcDiff();
      break;
    case 3:
      calcProd();
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
  mainMenu();
}
