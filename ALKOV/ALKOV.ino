#include <Keypad.h>  // Tuş takımı için kütüphane
#include <DHT.h>     // Nem sensörü için kütüphane

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
const char* password = "123456";

void setup() {
  pinMode(relayPin, OUTPUT);  // Röle pinini çıkış olarak ayarlama
  digitalWrite(relayPin, LOW);  // Röleyi kapalı konumda başlatma

  Serial.begin(9600);  // Seri haberleşme başlatma
  dht.begin();         // Nem sensörünü başlatma

  Serial.println("Enter password:");  // Şifre girişine hazır olma mesajı
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
        Serial.println("Password correct");
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
    Serial.println("Relay opened");
    passwordCorrect = false; // Röle kapatıldıktan sonra şifreyi sıfırlama
  }
}
