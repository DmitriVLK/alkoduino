#include <LiquidCrystal.h>
#include <OneWire.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#define ONE_WIRE_BUS 2 // номер пина к которому подключен DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
// put your setup code here, to run once:
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
const int buzzer = 3; //buzzer to arduino pin 3
int addrLowerBound = 0;
int addrUpperBound = 1;
byte lowerBoundTemp = 35;
byte upperBoundTemp = 40;

void setup() {
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  lcd.begin(16, 2);
  lcd.print ("Arduino Master!");
  sensors.begin(); 
  lowerBoundTemp = EEPROM.read(addrLowerBound);
  upperBoundTemp = EEPROM.read(addrUpperBound);
}

void loop() {
  sensors.requestTemperatures();
  int currentTemperature = sensors.getTempCByIndex(0);
  if(currentTemperature > upperBoundTemp || currentTemperature < lowerBoundTemp) {
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(1000);         // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);        // ...for 1sec
    int keyAnalog =  analogRead(A0);
    int counts = 5;
    while(keyAnalog < 800) {
      readKey(keyAnalog);
      updateScreen();
      if(counts>0){
        counts--;
        delay (700);
      } else {
        delay(50);
      }
      keyAnalog =  analogRead(A0);
    }
  } else {
    noTone(buzzer);     // Stop sound...
    delay(100);        // ...for 1sec
    int keyAnalog =  analogRead(A0);
    while(keyAnalog < 800) {
      readKey(keyAnalog);
      updateScreen();
      int counts = 5;
      if(counts>0){
        counts--;
        delay (700);
      } else {
        delay(50);
      }
      keyAnalog =  analogRead(A0);
    }
  }
  int keyAnalog =  analogRead(A0);
  readKey(keyAnalog);
  updateScreen();
}

void readKey(int keyAnalog)
{
  if (keyAnalog < 100) {
    // Значение меньше 100 – нажата кнопка right
    // Выполняем какое-то действие для кнопки вправо.
    upperBoundTemp = upperBoundTemp + 1;
    EEPROM.write(addrUpperBound, upperBoundTemp);
  } else if (keyAnalog < 200) {
    // Значение больше 100 (иначе мы бы вошли в предыдущий блок результата сравнения, но меньше 200 – нажата кнопка UP
    // Выполняем какое-то действие для кнопки вверх
    lowerBoundTemp = lowerBoundTemp + 1;
    EEPROM.write(addrLowerBound, lowerBoundTemp);
  } else if (keyAnalog < 400) {
    // Значение больше 200, но меньше 400 – нажата кнопка DOWN
    // Выполняем действие для кнопки вниз
    lowerBoundTemp = lowerBoundTemp - 1;
    EEPROM.write(addrLowerBound, lowerBoundTemp);
  } else if (keyAnalog < 600) {
    // Значение больше 400, но меньше 600 – нажата кнопка LEFT
    // Выполняем действие для кнопки влево
    upperBoundTemp = upperBoundTemp - 1;
    EEPROM.write(addrUpperBound, upperBoundTemp);
  } else if (keyAnalog < 800) {
    // Значение больше 600, но меньше 800 – нажата кнопка SELECT
    // Выполняем действие для кнопки выбора пункта меню
  } else {
    // Все остальные значения (до 1023) будут означать, что нажатий не было
  }
}

void updateScreen()
{
    // put your main code here, to run repeatedly:
  sensors.requestTemperatures();
  int currentTemperature = sensors.getTempCByIndex(0);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(currentTemperature); 
  lcd.print(" *C");
  lcd.setCursor(0, 1);
  lcd.print("LB: ");
  lcd.print(lowerBoundTemp);
  lcd.print("UB: ");
  lcd.print(upperBoundTemp);
}

