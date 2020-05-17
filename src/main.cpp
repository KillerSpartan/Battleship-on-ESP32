#include <Arduino.h>
#include <Wire.h>
#include "stdlib.h"
#include "stdio.h"
#include "LiquidCrystal_I2C.h"

//Todos los caracteres customizados
byte frame[] = {
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111
};
byte ALetter[] = {
  B00000,
  B11100,
  B10100,
  B11100,
  B10100,
  B10100,
  B00000,
  B00000
};
byte BLetter[] = {
  B00000,
  B11000,
  B10100,
  B11100,
  B10100,
  B11000,
  B00000,
  B00000
};
byte barcoVertical[] = {
  B00000,
  B01110,
  B01110,
  B01010,
  B01010,
  B01010,
  B00100,
  B00000
};
byte back_BarcoHorizontal[] = {
  B00000,
  B00000,
  B11111,
  B10000,
  B10111,
  B10000,
  B11111,
  B00000
}; //1
byte front_BarcoHorizontal[] = {
  B00000,
  B00000,
  B11100,
  B00110,
  B11011,
  B00110,
  B11100,
  B00000}; //2
byte bFont[] = {
  B11110,
  B11001,
  B01001,
  B11111,
  B11001,
  B01001,
  B11001,
  B11110
};
byte null[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};


//Configuracion de LCD
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows);

//Variables Globales
#define ADC 4
#define buttonStart 5
#define buttonAction 18
#define buttonPut 19


int sel = 0;
int Value = 0;
int positionY = 0;
int positionX = 2;
int oneX = 18, oneY = 2;
char Position[14];
char incomingByte;

void creadorCustom(){
  lcd.createChar(1, back_BarcoHorizontal);
  lcd.createChar(2, front_BarcoHorizontal);
  lcd.createChar(3, barcoVertical);
  lcd.createChar(4, ALetter);
  lcd.createChar(5, BLetter);
  lcd.createChar(6, bFont);
  lcd.createChar(7, null);
  lcd.createChar(8, frame);
}
void startScreen(){
  lcd.setCursor(2,0);
  lcd.write(6);
  lcd.print("attleship");

  lcd.setCursor(0,1);
  lcd.print("Press = to start");

}

int frameForGame(int x, int y){
  lcd.setCursor(0,0);lcd.write(4);
  lcd.setCursor(0,1);lcd.write(5);
  lcd.setCursor(x,y);
  lcd.write(2);
  lcd.setCursor(x-1,y);lcd.write(1);

  for (int i = 1; i < 16; i++) {
    if((i!=oneX && i != x-1)){
      lcd.setCursor(i, 0);
      lcd.write(8);
      lcd.setCursor(i, 1);
      lcd.write(8);
    }
  }
  delay(50);
  return 0;

}
void posicionamiento(char ascii){

  switch (ascii) {
    case 's':
      positionY++;
    break;

    case 'w':
    positionY--;
    break;

    case 'a':
    positionX--;
    break;

    case 'd':
    positionX++;
    break;
  }

  if(positionX<1){positionX = 1;}
  if(positionX > 15){positionX = 15;}
  if(positionY>1){positionY=1;}
  if(positionY<0){positionY=0;}

  lcd.setCursor(positionX, positionY);
  lcd.write(2);
  lcd.setCursor(positionX-1, positionY);
  lcd.write(1);
  delay(10);

  if(digitalRead(buttonAction) == HIGH){delay(35);positionY++;if(positionY>1){positionY = 0;}}
}

void setup(){
  lcd.init();
  lcd.backlight();
  creadorCustom();
  startScreen();
  Serial.begin(9600);
}

void loop(){

  delay(10);

  Value  = (analogRead(ADC))/273;
  sprintf(Position, "%d", Value);


  if(digitalRead(buttonStart)==HIGH){delay(30);sel++;}
    switch (sel) {
      case 1:
        frameForGame(oneX,oneY);
        if (Serial.available() > 0) {
          incomingByte = Serial.read();
          posicionamiento(incomingByte);
          // say what you got:
         Serial.print("I received: ");
         Serial.println(incomingByte, DEC);
         delay(10);
        }
      break;

      case 2:
        oneX = positionX;
        oneY = positionY;
        sel = 1;
    }

    //Barco en Movimiento
    lcd.setCursor(positionX, positionY);
    lcd.write(2);
    lcd.setCursor(positionX-1, positionY);
    lcd.write(1);
    delay(20);
    //Barco Uno



    //~ case 3 ... 7: // Si alcanza cierto punto se reinicia el programa
      //~ loop();
      //~ sel=0;
    //~ break;

}
