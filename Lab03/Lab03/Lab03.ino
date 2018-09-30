const int sensorPin = A0;
const int greenPin = 9;
const int redPin = 11;
const int bluePin = 10;

int greenValue = 0;
int redValue = 0;
int blueValue = 255;

int baselineValue;
float baselineTemp;

const int rollingAvgNum = 5;
int rollingAvg[rollingAvgNum];
int readNum;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  createBaselineValue(10);
  baselineTemp = convertValtoTemp(baselineValue);
}

void createBaselineValue(int numReadings){
  int sum = 0;
  int value;
  for(int i = 0; i < numReadings; i++){
    value = getValue();
    addToAvg(value);
    sum += value;
    delay(250);
  }
  baselineValue = sum / numReadings;
}

void addToAvg(int reading){
  rollingAvg[readNum % rollingAvgNum] = reading;
  readNum++;
  if(readNum >= 5){
    readNum = 0;
  }
}

int calcAvg(){
  int sum = 0;
  for(int i = 0; i < rollingAvgNum; i++){
    sum += rollingAvg[i];
  }
  return sum / rollingAvgNum;
}

int getValue(){
  return analogRead(sensorPin);  
}

float convertValtoTemp(int sensorVal){
  float voltage = (sensorVal / 1024.0) * 5.0;
  float temp = (voltage - 0.5) * 100;
  return temp;
}

void printInfo(int sensorVal, int normalizedVal, float voltage, float temp, float tempDiff){
  Serial.print("Sensor Value: ");
  Serial.print(sensorVal);
  Serial.print(", Normalized SensorValue: ");
  Serial.print(normalizedVal);
  Serial.print(", Voltage: ");
  Serial.print(voltage);
  Serial.print(", Degrees C: ");
  Serial.print(temp);
  Serial.print(", Difference C: ");
  Serial.print(tempDiff);
  Serial.print(" Red Value: ");
  Serial.print(redValue);
  Serial.print(" Blue Value: ");
  Serial.println(blueValue);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorVal = getValue();
  addToAvg(sensorVal);
  int normalizedVal = calcAvg();
  float voltage = (normalizedVal / 1024.0) * 5.0;
  float temp = (voltage - 0.5) * 100;
  float tempDiff = temp - baselineTemp;

// added a weight to the difference to make the percent more pernounced
  float percentDif = (max(0, tempDiff) * 2) / baselineTemp;
  redValue = min(255, 255 * percentDif);
  blueValue = min(255, 255 * ( 1 - percentDif));

  analogWrite(redPin, redValue);
  analogWrite(bluePin, blueValue);
  
  printInfo(sensorVal, normalizedVal, voltage, temp, tempDiff);
}
