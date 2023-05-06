#include <Keypad.h>  // Tuş takımı için kütüphane
#include <DHT.h>     // Nem sensörü için kütüphane
#include <Wire.h>    // I2C kütüphanesi
#include <LiquidCrystal_I2C.h>   // I2C LCD kütüphanesi

// I2C adresi ve satır/sütun sayısı tanımlama
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Tuş takımı tanımlamaları
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Sensörlerin pinleri tanımlama
const int distanceSensorPin = A0;
const int gasSensorPin = A1;
const int humiditySensorPin = A2;

// Role bağlı cihazların pinleri tanımlama
const int relayPin = 2;

// Nem sensörü tanımlamaları
#define DHTTYPE DHT11   // Nem sensörü tipi
DHT dht(humiditySensorPin, DHTTYPE);

// Şifre tanımlama
const char* password = "341453";

void setup() {
  
  lcd.init();       // LCD ekranını başlatma
  lcd.backlight();  // Arka aydınlatmayı açma
  lcd.setCursor(0, 0);  // İlk satıra gitme
  lcd.print("ALKOV SİSTEMİ");  // Yazıyı yazdırma

  pinMode(relayPin, OUTPUT);  // Röle pinini çıkış olarak ayarlama
  digitalWrite(relayPin, LOW);  // Röleyi kapalı konumda başlatma

  Serial.begin(9600);  // Seri haberleşme başlatma
  dht.begin();         // Nem sensörünü başlatma

  Serial.println("Sifre giris:");  // Şifre girişine hazır olma mesajı
}

void loop() {
  char key = keypad.getKey();  // Tuş takımından gelen veriyi okuma

  static int passwordIndex = 0;
  static bool passwordCorrect = false;
  if (key) {
    if (key == password[passwordIndex]) {
      passwordIndex++;
      if (passwordIndex == strlen(password)) {
        passwordCorrect = true;
        Serial.println("sifre dogru");      
        lcd.setCursor(0, 1);  // İlk satıra gitme
        lcd.print("Giris basarili");  // Yazıyı yazdırma
      }
    } else {
      passwordIndex = 0;
    }
  }

  // Sensörlerden veri okuma
  int distance = analogRead(distanceSensorPin);
  int gas = analogRead(gasSensorPin);
  float humidity = dht.readHumidity();

  // Role'ü açma
  if (distance < 60 && humidity > 20 && gas < 200 && passwordCorrect) {
    digitalWrite(relayPin, HIGH);
    Serial.println("Role acildi");
    lcd.setCursor(0, 1);  // İlk satıra gitme
    lcd.print("Araba calisabilir");  // Yazıyı yazdırma
    //passwordCorrect = false; // Röle kapatıldıktan sonra şifreyi sıfırlama
  }
  else{
    lcd.setCursor(0, 1);  // İlk satıra gitme
    lcd.print("Giris basarisiz");  // Yazıyı yazdırma
  }
}
