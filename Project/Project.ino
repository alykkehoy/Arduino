// include the library code:
#include <LiquidCrystal.h>

const int CONTRAST_PIN = 6;
int contrastValue = 50;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int BUTTON1_PIN = 10;
const int BUTTON2_PIN = 9;

int b1State;
int b2State;


void setup() {
  pinMode(CONTRAST_PIN, OUTPUT);
  analogWrite(CONTRAST_PIN, contrastValue); 
  lcd.begin(16, 2);
  lcd.noAutoscroll();

  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  Serial.begin(9600);
}


//Display the intro splash screen
void splashScreen(){
  lcd.clear();
  lcd.print("Welcome");
  lcd.setCursor(0, 1);
  lcd.print("User!");

  Serial.print("Welcome User!\n");

  b1State = digitalRead(BUTTON1_PIN);
  b2State = digitalRead(BUTTON2_PIN);
  //Wait for both buttons to be in a pressed state
  while((b1State == LOW) || (b2State == LOW)){
    b1State = digitalRead(BUTTON1_PIN);
    b2State = digitalRead(BUTTON2_PIN);
  }
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
void calculator(){
  //get the 2 nubmers to be added
  int num1 = getNum(1);
  int num2 = getNum(2);

  int sum = num1 + num2;
  lcd.clear();
  lcd.print("Sum is:");
  Serial.print("Sum is:\n");
  lcd.setCursor(0,1);

  //for every space on the bottom row of the lcd, read the corresponding bit from the sum
  for (int i = 15; i>=0; i--){
    if (bitRead(sum,i)==1){
      lcd.print ("1");
      Serial.print("1");
    }else{
      lcd.print ("0");
      Serial.print("0");
    }
  }

  b1State = digitalRead(BUTTON1_PIN);
  b2State = digitalRead(BUTTON2_PIN);
  //wait for both buttons to be in the pressed state
  while((b1State == LOW) || (b2State == LOW)){
    b1State = digitalRead(BUTTON1_PIN);
    b2State = digitalRead(BUTTON2_PIN);
  }
}

//Display the contrast screen
void contrastScreen(){
  lcd.clear();
  lcd.print("Contrast: ");
  lcd.setCursor(0,1);

  Serial.print("Contrast:\n");
  
  b1State = digitalRead(BUTTON1_PIN);
  b2State = digitalRead(BUTTON2_PIN);

  bool b1Pressed = false;
  bool b2Pressed = false;
  while((b1State == LOW) || (b2State == LOW)){
    b1State = digitalRead(BUTTON1_PIN);
    b2State = digitalRead(BUTTON2_PIN);

    //if the right button is pressed increase the contrast
    if(b1State != LOW && !b1Pressed){
      b1Pressed = true;
      contrastValue++;
      lcd.setCursor(0,1);
      lcd.print(contrastValue);
      Serial.print(String(contrastValue) + "\n");
    }
    //if the left button is pressed decrease the contrast
    else if(b2State != LOW && !b2Pressed){
      b2Pressed = true;
      contrastValue--;
      lcd.setCursor(0,1);
      lcd.print(contrastValue);
      Serial.print(String(contrastValue) + "\n");
    }

    //reset button 2 pressed boolean
    if(b2State == LOW){
      b2Pressed = false;
    }

    //reset button 1 pressed boolean
    if(b1State == LOW){
      b1Pressed = false;
    }

    //write the new contrast value
    analogWrite(CONTRAST_PIN, contrastValue); 
  }
}

void loop() {
  splashScreen();
  calculator();
  delay(2000);
  contrastScreen();
}
