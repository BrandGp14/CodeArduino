#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int stepperPins[4] = {4, 5, 6, 7};
int currentStep = 0, pStep = 0;

int counter = 0;
int p_counter = -1;

bool sensorState_1_sol = false;
bool sensorState_2_sol = false;
bool sensorState_5_sol = false;

int sensorThreashHold = 75; 

long started_at;

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.setBacklight(1);
  lcd.setCursor(0, 0);
  lcd.print("----ALCANCIA----");

  for (int i = 0; i < 4; i++) {
    pinMode(stepperPins[i], OUTPUT);
  }

  started_at = millis();
}

void loop() {
  int sensorValue_1_sol = analogRead(A2);
  int sensorValue_2_sol = analogRead(A3);
  int sensorValue_5_sol = analogRead(A5);

  Serial.print("Sensor: ");
  Serial.print(sensorValue_1_sol);
  Serial.print(", ");
  Serial.print(sensorValue_2_sol);
  Serial.print(", ");
  Serial.println(sensorValue_5_sol);

  if (sensorValue_1_sol < sensorThreashHold && !sensorState_1_sol) counter += 5;
  if (sensorValue_2_sol < sensorThreashHold && !sensorState_2_sol) counter += 2;
  if (sensorValue_5_sol < sensorThreashHold && !sensorState_5_sol) counter += 1;

  sensorState_1_sol = sensorValue_1_sol < sensorThreashHold;
  sensorState_2_sol = sensorValue_2_sol < sensorThreashHold;
  sensorState_5_sol = sensorValue_5_sol < sensorThreashHold;

  if (counter != p_counter && sensorValue_5_sol > 700) {
    printTotal();
    p_counter = counter;
  }

  moveStepperMotor();
}

void printTotal() {
  lcd.setCursor(0, 1);
  lcd.print("                "); 
  lcd.setCursor(0, 1);
  lcd.print("Total:S/");
  lcd.print(counter);
}

void moveStepperMotor() {
  for (int i = 0; i < 20; i++) {
    digitalWrite(stepperPins[i], (i == currentStep) ? HIGH : LOW);
  }

  pStep = currentStep;
  currentStep = (currentStep + 1) % 4;

  delay(1); 
}