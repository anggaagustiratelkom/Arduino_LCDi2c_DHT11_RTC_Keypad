#include <virtuabotixRTC.h>
virtuabotixRTC myRTC(10, 11, 12); //CLK, DAT, RST

#include <DHT.h>
DHT dht(13, DHT11); //Pin, Jenis DHT
  
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,  2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#include <Keypad.h>
#define Password_Lenght 8
#define ganti_lenght 1

char Data[Password_Lenght]; // 6 is the number of chars it can hold + the null char = 7
char Master[Password_Lenght] = "A010899"; 
byte data_count = 0,    master_count = 0;
char customKey;
char pindah;
char ganti[ganti_lenght];
byte ganti_count = 1;  

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {
  lcd.begin(16,2);
  Serial.begin(9600);
  dht.begin();
  //myRTC.setDS1302Time(00, 46, 17, 6, 18, 01, 2019); // Set Waktu(detik,menit,jam,  ,hari,bulan,tahun)
}

void loop() {
  while(!((data_count == Password_Lenght-1)&&(!strcmp(Data,Master)))){ 
    lcd.setCursor(0,0);
    lcd.print("Msukan Password");
    lcd.setCursor(0,1);
    lcd.print("Pass : ");
    customKey = customKeypad.getKey();
    if (customKey) // makes sure a key is actually pressed, equal to (customKey != NO_KEY)
    {
      Data[data_count] = customKey; // store char into data array
      lcd.setCursor(data_count+8,1); // move cursor to show each new char
      lcd.print(Data[data_count]); // print char at said cursor
      data_count++;
      if((data_count == Password_Lenght-1)&&(!strcmp(Data,Master))){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Selamat datang");
        lcd.setCursor(0,1);
        lcd.print("Angga Agustira"); 
        delay(5000);
        lcd.clear(); 
      }
      else if(((data_count == Password_Lenght-1)&&(strcmp(Data,Master)))){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Password salah");
        lcd.setCursor(0,1);
        lcd.print("Slhkn coba lagi");
        delay(1000);
        lcd.clear();
        clearData();
      }
    }
  }
  while((ganti[1] != 'A')&&(ganti[1]!= 'B')){
    lcd.setCursor(0,0);
    lcd.print("A.Mod Jam");
    lcd.setCursor(0,1);
    lcd.print("B.Mod Suhu Rngn");
    pindah = customKeypad.getKey(); 
    if(pindah){
      ganti_count++;
      ganti[1] = pindah;
      lcd.clear();
    }
  }
  if(ganti[1] == 'A'){
     myRTC.updateTime();
     lcd.setCursor(0,0);
     lcd.print("Tgl : ");
     lcd.print(myRTC.dayofmonth);
     lcd.print("/");
     lcd.print(myRTC.month);
     lcd.print("/");
     lcd.print(myRTC.year);
     lcd.print(" ");
     lcd.setCursor(0,1);
     lcd.print("Jam : ");
     lcd.print(myRTC.hours);
     lcd.print(":");
     lcd.print(myRTC.minutes);
     lcd.print(":");
     lcd.print(myRTC.seconds);
     lcd.println(' ');
  }  
  else if(ganti[1] == 'B'){
    float kelembaban = dht.readHumidity();
    float suhu = dht.readTemperature();
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);   
    lcd.print("Klmbbn : ");
    lcd.setCursor(9,0);
    lcd.print(kelembaban);
    lcd.setCursor(15,0);
    lcd.write(B00100101);
    lcd.setCursor(0,1);
    lcd.print("Suhu   : ");
    lcd.setCursor(9,1);
    lcd.print(suhu);
    lcd.setCursor(14,1);
    lcd.write(B11011111);
    lcd.setCursor(15,1);
    lcd.print("C");
    lcd.println(' ');
    }
}

void clearData()
{
  while(data_count !=0)
  {   // This can be used for any array size, 
    Data[data_count--] = 0; //clear array for new data
  }
  return;
}

