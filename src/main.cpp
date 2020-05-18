#include <Arduino.h>
#include <Wire.h>
#include "stdlib.h"
#include "stdio.h"
#include "LiquidCrystal_I2C.h"
#include <bits.h>
#include <iostream>
#include <algorithm>


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
  B01110,
  B10001,
  B11011,
  B10001,
  B01010,
  B01110,
  B01110,
  B00000
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
#define LED 2
#define buttonStart 5

//Para el LED
int brightness = 0;
int fadeAmount = 5;

const int freq=5000;
const int ledChannel = 0;
const int resolution = 10;


int sel = 0;
int Value = 0;
int positionY = 0;
int positionX = 2;

//bombas
int bombX = 18, bombY=2;


//Barcos for Player 1
int oneX = 18, oneY = 2;
int twoX = 18, twoY=2;
int threeX = 18, threeY = 2;

//Barcos for Player 2
int p2_oneX = 18, p2_oneY = 2;
int p2_twoX = 18, p2_twoY=2;
int p2_threeX = 18, p2_threeY = 2;

//Player one
int P1BarcoOne[]= {0, 0};
int P1BarcoTwo[]= {0, 0};
int P1BarcoTre[]= {0, 0};
int P1Bomb[]={0, 0};
char P1Name[10];

//Player 2
int P2BarcoOne[]= {0, 0};
int P2BarcoTwo[]= {0, 0};
int P2BarcoTre[]= {0, 0};
char P2Name[10];

int numBP1 = 0, numBP2 = 0; //Cantidad de Barcos
int numBomb = 0; //Cantidad de cbombas
char Position[14];
char incomingByte;

void PWMStart(){
  pinMode(LED, OUTPUT);

ledcSetup(ledChannel, freq, resolution);
ledcAttachPin(LED, ledChannel);

}

bool areEqual(int arr1[], int arr2[])
{

    // Linearly compare elements
    for (int i = 0; i < 2; i++)
        if (arr1[i] != arr2[i])
            return false;

    // If all elements were same.
    return true;
}


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
  return 0;} //Esta es la pantalla para P1
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
   if(numBP2==3){x=p2_threeX; y=p2_threeY;}
   lcd.write(3);
   break;
 }

 if(numBP2==1){x=p2_oneX; y=p2_oneY;}
 if(numBP2==2){x=p2_twoX; y=p2_twoY;}

 lcd.setCursor(p2_oneX,p2_oneY);
 lcd.write(2);
 lcd.setCursor(p2_oneX-1,p2_oneY);
 lcd.write(1);

 lcd.setCursor(p2_twoX,p2_twoY);
 lcd.write(2);
 lcd.setCursor(p2_twoX-1,p2_twoY);
 lcd.write(1);

 lcd.setCursor(p2_threeX, p2_threeY);
 lcd.write(3);

//(i!=oneX) && (i != oneX-1) && (i!=twoX) && (i!=twoX-1) && (i!=threeX) && (i!=oneY) && (i!=twoY)
  for (int i = 1; i < 16; i++) {
    if((i!=p2_oneX && i!=p2_oneX-1)&&(i!=p2_twoX && i!=p2_twoX-1)&&(i!=p2_threeX)){
      lcd.setCursor(i, 0);
      lcd.write(8);
      lcd.setCursor(i, 1);
      lcd.write(8);
    }
  }
  delay(50);
  return 0;} //Esta es la pantalla para P2
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
  if(positionX>15){positionX = 15;}
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

//Frames para bombas
int bombardeoP1(char ascii){
  delay(1000);
  lcd.setCursor(0,0);lcd.write(4);
  lcd.setCursor(0,1);lcd.write(5);

  switch (ascii) {
    case 'a':
    bombY = 0;
    break;

    case 'b':
    bombY = 1;
    break;

    case '1':
    bombX = 1;
    break;

    case '2':
    bombX = 2;
    break;

    case '3':
    bombX = 3;
    break;

    case '4':
    bombX = 4;
    break;

    case '5':
    bombX = 5;
    break;

    case '6':
    bombX = 6;
    break;

    case '7':
    bombX = 7;
    break;

    case '8':
    bombX = 8;
    break;

    case '9':
    bombX = 9;
    break;

    case '0':
    bombX = 10;
    break;


  }

  lcd.setCursor(bombX, bombY);
  lcd.write(6);

  P1Bomb[0] = bombX;
  P1Bomb[1] = bombY;

   if(areEqual(P1Bomb, P2BarcoOne)){
     bombX = 18;   bombY=2;
     Serial.println("Yes");
     lcd.noBlink();
     delay(500);
     lcd.blink();
     delay(3000);
     numBP2--;
     numBomb++;
     P2BarcoOne[0]=18; P2BarcoOne[1]=2;
   }else{Serial.println("No le diste tronco");}

   if(areEqual(P1Bomb, P2BarcoTwo)){
     bombX = 18;   bombY=2;
     Serial.println("Yes");
     lcd.noBlink();
     delay(500);
     lcd.blink();
     delay(3000);
     numBP2--;
     numBomb++;
     P2BarcoTwo[0]=18; P2BarcoTwo[1]=2;
   }else{Serial.println("No le diste tronco");}

   if(areEqual(P1Bomb, P2BarcoTre)){
     bombX = 18;   bombY=2;
     Serial.println("¡¡Le has dado!! uWu");
     lcd.noBlink();
     delay(500);
     lcd.blink();
     delay(3000);
     numBP2--;
     numBomb++;
     P2BarcoTre[0]=18; P2BarcoTre[1]=2;
   }else{Serial.println("No le diste tronco");}


if(numBP2 == 0 && numBomb == 3){
  sel=30;
}

for (int i = 1; i < 16; i++) {
if(i!=bombX){
  lcd.setCursor(i, 0);
  lcd.write(8);
  lcd.setCursor(i, 1);
  lcd.write(8);
}

  }
  return 0;
}

void bombardeoP2(){
  lcd.setCursor(0,0);lcd.write(4);
  lcd.setCursor(0,1);lcd.write(5);
  for (int i = 1; i < 16; i++) {
      lcd.setCursor(i, 0);
      lcd.write(8);
      lcd.setCursor(i, 1);
      lcd.write(8);
  }
}

void setup(){
  lcd.init();
  lcd.backlight();
  creadorCustom();
  startScreen();
  Serial.begin(9600);
  PWMStart();
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
        positionX = 18;
        positionY=2;

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
      delay(2000);
      positionX=2;
      positionY=0;
      lcd.setCursor(0, 0);
      lcd.print("Es turno de P2");
      lcd.setCursor(0, 1);
      lcd.print("Preparate dude");
      delay(1000);
      sel = 9;
      break;

      case 9:

        P2ForGame(p2_oneX,p2_oneY, 0);
        if (Serial.available() > 0) {
          incomingByte = Serial.read();
          posicionamiento(incomingByte);
                // say what you got:
          Serial.print("I received: ");
          Serial.println(incomingByte, DEC);
          delay(10);
        }
      break;

      case 10:
      p2_oneX = positionX;
      p2_oneY = positionY;
      P2ForGame(p2_oneX,p2_oneY, 0);
      numBP2++;
      sel = 11;
      break;

      case 11:
      P2ForGame(p2_twoX,p2_twoY, 0);
      if (Serial.available() > 0) {
        incomingByte = Serial.read();
        posicionamiento(incomingByte);
        // say what you got:
       Serial.print("I received: ");
       Serial.println(incomingByte, DEC);
       delay(10);}
      break;

      case 12:
      p2_twoX = positionX;
      p2_twoY = positionY;
      P2ForGame(p2_twoX,p2_twoY, 0);
      numBP2++;
      sel = 13;
      break;

      case 13:
      P2ForGame(p2_threeX,p2_threeY, 1);
      if (Serial.available() > 0) {
        incomingByte = Serial.read();
        posicionlast(incomingByte);
        // say what you got:
       Serial.print("I received: ");
       Serial.println(incomingByte, DEC);
       delay(10);
      }
      break;

      case 14:
      p2_threeX = positionX;
      p2_threeY = positionY;
      P2ForGame(p2_threeX,p2_threeY, 1);
      numBP2++;
      sel=15;
      break;

      case 15:
      P2BarcoOne[0]=p2_oneX; P2BarcoOne[1] = p2_oneY;
      P2BarcoTwo[0]=p2_twoX; P2BarcoTwo[1] = p2_twoY;
      P2BarcoTre[0]=p2_threeX; P2BarcoTre[1] = p2_threeY;

      //lcd.clear();
      //delay(3800);
      Serial.println("Ubicacion Barco Uno");
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

      Serial.println("Ubicacion Barco Dos");
      Serial.print(P2BarcoOne[0]);
      Serial.print(",");
      Serial.print(P2BarcoOne[1]);
      Serial.println("  Barco Uno (P2)");
      delay(400);

      Serial.print(P2BarcoTwo[0]);
      Serial.print(",");
      Serial.print(P2BarcoTwo[1]);
      Serial.println("  Barco Dos(P2)");
      delay(400);

      Serial.print(P2BarcoTre[0]);
      Serial.print(",");
      Serial.print(P2BarcoTre[1]);
      Serial.println("  Barco Tres(P2)");
      delay(400);

      sel=16;

      //sel = 16;
      break;

      case 16:
      lcd.clear();
      delay(500);
      positionX=18; positionY=2;
      sel=17;
      break;

      case 17:
      lcd.setCursor(0,0);
      lcd.blink();
      lcd.print("Game Start");
      delay(1000);
      sel=18;
      break;

//·············Estos son los TURNOS de cada jugador················

      //Player One Turno
      case 18:
      lcd.noBlink();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Turno de P1");
      Serial.println("Es turno de P1");
      delay(1000);
      sel=19;
      break;

      case 19:

      bombardeoP1(incomingByte);

      if (Serial.available() > 0) {
        incomingByte = Serial.read();
        Serial.print("I received: ");
        Serial.println(incomingByte, DEC);
        delay(10);
      }

      if(numBP2 > 2){Value = 1024;}
      if(numBP2 == 2){Value = 500;}
      if(numBP2 == 1){Value = 250;}
      if(numBP2 < 1){Value = 0;}
      ledcWrite(ledChannel, Value);

      break;



      //~ //Player Two Turno
      case 20:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Turno de P2");
      Serial.println("Es turno de P2");
      delay(1000);
      sel=21;
      break;

      case 21:
        bombardeoP2();
        delay(2000);
        sel=18;
      break;


      ///*****GAME OVER////

      case 30:
      lcd.clear();
      delay(1000);
      sel = 31;
      break;

      case 31:
      lcd.setCursor(0, 0);
      lcd.print("Game Over");
      lcd.setCursor(0, 1);
      lcd.print("P1 Wins");
      Serial.println("El jugador P1 ganó");



}



//·········Movimiento de barcos en la LCD········33

if((numBP1>1 && numBP2<1) || (numBP1 >1 && numBP2>1)){lcd.setCursor(positionX, positionY); lcd.write(3); delay(50);}
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
