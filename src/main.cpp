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


int sel = 0;
int Value = 0;
int positionY = 0;
int positionX = 2;

//Barcos
int oneX = 18, oneY = 2;
int twoX = 18, twoY=2;
int threeX = 18, threeY = 2;

//Player one
int P1BarcoOne[]= {0, 0};
int P1BarcoTwo[]= {0, 0};
int P1BarcoTre[]= {0, 0};


int numBP1 = 0;
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

int frameForGame(int x, int y, int size){

  lcd.setCursor(0,0);lcd.write(4);
  lcd.setCursor(0,1);lcd.write(5);

 switch(size) {
   case 0:
   lcd.setCursor(x,y);
   lcd.write(2);
   lcd.setCursor(x-1,y);
   lcd.write(1);

   //Serial.print(twoX);Serial.print(twoY);
   break;

   case 1 :
   lcd.setCursor(x,y);
   if(numBP1==3){x=threeX; y=threeY;}
   lcd.write(3);
   break;
 }

 if(numBP1==1){x=oneX; y=oneY;}
 if(numBP1==2){x=twoX; y=twoY;}

 lcd.setCursor(oneX,oneY);
 lcd.write(2);
 lcd.setCursor(oneX-1,oneY);
 lcd.write(1);

 lcd.setCursor(twoX,twoY);
 lcd.write(2);
 lcd.setCursor(twoX-1,twoY);
 lcd.write(1);

 lcd.setCursor(threeX, threeY);
 lcd.write(3);

//(i!=oneX) && (i != oneX-1) && (i!=twoX) && (i!=twoX-1) && (i!=threeX) && (i!=oneY) && (i!=twoY)
  for (int i = 1; i < 16; i++) {
    if((i!=oneX && i!=oneX-1)&&(i!=twoX && i!=twoX-1)&&(i!=threeX)){
      lcd.setCursor(i, 0);
      lcd.write(8);
      lcd.setCursor(i, 1);
      lcd.write(8);
    }
  }
  delay(50);
  return 0;
} //Esta es la pantalla para P1
int P2ForGame(int x, int y, int size){

  lcd.setCursor(0,0);lcd.write(4);
  lcd.setCursor(0,1);lcd.write(5);

 switch(size) {
   case 0:
   lcd.setCursor(x,y);
   lcd.write(2);
   lcd.setCursor(x-1,y);
   lcd.write(1);

   //Serial.print(twoX);Serial.print(twoY);
   break;

   case 1 :
   lcd.setCursor(x,y);
   if(numBP1==3){x=threeX; y=threeY;}
   lcd.write(3);
   break;
 }

 if(numBP1==1){x=oneX; y=oneY;}
 if(numBP1==2){x=twoX; y=twoY;}

 lcd.setCursor(oneX,oneY);
 lcd.write(2);
 lcd.setCursor(oneX-1,oneY);
 lcd.write(1);

 lcd.setCursor(twoX,twoY);
 lcd.write(2);
 lcd.setCursor(twoX-1,twoY);
 lcd.write(1);

 lcd.setCursor(threeX, threeY);
 lcd.write(3);

//(i!=oneX) && (i != oneX-1) && (i!=twoX) && (i!=twoX-1) && (i!=threeX) && (i!=oneY) && (i!=twoY)
  for (int i = 1; i < 16; i++) {
    if((i!=oneX && i!=oneX-1)&&(i!=twoX && i!=twoX-1)&&(i!=threeX)){
      lcd.setCursor(i, 0);
      lcd.write(8);
      lcd.setCursor(i, 1);
      lcd.write(8);
    }
  }
  delay(50);
  return 0;
} //Esta es la pantalla para P2
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
}
void posicionlast(char ascii){
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
  lcd.write(3);
  delay(50);
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
  if(digitalRead(buttonStart)==HIGH){delay(30);sel++;}
    switch (sel) {
      case 1:

        frameForGame(oneX,oneY, 0);
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
        frameForGame(oneX,oneY, 0);
        numBP1++;
        sel = 3;
      break;
      case 3:
      frameForGame(twoX,twoY, 0);
      if (Serial.available() > 0) {
        incomingByte = Serial.read();
        posicionamiento(incomingByte);
        // say what you got:
       Serial.print("I received: ");
       Serial.println(incomingByte, DEC);
       delay(10);
     }
        break;
      case 4:
        twoX = positionX;
        twoY = positionY;
        frameForGame(twoX,twoY, 0);
        numBP1++;
        sel = 5;
        break;
      case 5:
      frameForGame(threeX,threeY, 1);
      if (Serial.available() > 0) {
        incomingByte = Serial.read();
        posicionlast(incomingByte);
        // say what you got:
       Serial.print("I received: ");
       Serial.println(incomingByte, DEC);
       delay(10);
      }
      break;
      case 6:
        threeX = positionX;
        threeY = positionY;
        frameForGame(threeX,threeY, 1);
        sel=7;
      break;
      case 7:

        P1BarcoOne[0]=oneX; P1BarcoOne[1] = oneY;
        P1BarcoTwo[0]=twoX; P1BarcoTwo[1] = twoY;
        P1BarcoTre[0]=threeX; P1BarcoTre[1] = threeY;

        lcd.clear();
        delay(3800);

        Serial.print(P1BarcoOne[0]);
        Serial.print(",");
        Serial.print(P1BarcoOne[1]);
        Serial.println("  Barco Uno");
        delay(400);

        Serial.print(P1BarcoTwo[0]);
        Serial.print(",");
        Serial.print(P1BarcoTwo[1]);
        Serial.println("  Barco Dos");
        delay(400);

        Serial.print(P1BarcoTre[0]);
        Serial.print(",");
        Serial.print(P1BarcoTre[1]);
        Serial.println("  Barco Tres");
        delay(400);

        sel = 8;
        break;
//·················Cambio de Turno···························
      case 8:
      lcd.setCursor(0, 0);
      lcd.print("Es turno de P2");
      lcd.setCursor(0, 1);
      lcd.print("Preparate dude");
      delay(1000);
      sel = 9;
      break;

      case 9:
      break;
}


if(numBP1>1){lcd.setCursor(positionX, positionY); lcd.write(3); delay(50);}
else{
  lcd.setCursor(positionX, positionY);
  lcd.write(2);
  lcd.setCursor(positionX-1, positionY);
  lcd.write(1);
  delay(20);
}



delay(100);



    //Barco Uno



    //~ case 3 ... 7: // Si alcanza cierto punto se reinicia el programa
      //~ loop();
      //~ sel=0;
    //~ break;


}
