#include <HCSR04.h>

#include "deneyap.h"
#include "DHT.h"
#define DHTPIN D0
#define DHTTYPE DHT11
String girilen = "";
DHT dht(DHTPIN, DHTTYPE);
bool kontrol, onay, kimlik;
#include "Keypad.h"
const byte satirsayisi = 4;  // satır sayısı
const byte sutunsayisi = 3;  // sütun sayısı

char tustakimi[satirsayisi][sutunsayisi] = {  // Tuş takımının görüntüsünü
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte satirpinleri[satirsayisi] = { D0, D1, D4, D5 };  // satır pinleri
byte sutunpinleri[sutunsayisi] = { D6, D7, D8 };      //sütun pinleri

Keypad keypad = Keypad(makeKeymap(tustakimi), satirpinleri, sutunpinleri, satirsayisi, sutunsayisi);
int gaspin = A0;
int esikDegeri = 400;  //Gaz eşik değerini belirle

//#include "hcsr04ESP32.h"
#define trigPin D14
#define echoPin D15
int mesafe;
int esikMesafe;
UltraSonicDistanceSensor mesafesensor(trigPin, echoPin);

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(gaspin, OUTPUT);


  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {

  sifreKontrol(false);
}

void sifreKontrol(bool kimlik) {
  String sifre = "041220222";
  //veri tabanında al bu şifreyi

  int a = 0;
  while (a < 10) {
    char basilantus = keypad.getKey();

    if (basilantus != NO_KEY) {  // basilantus değişkeni ile no_key(key yok ) eşit değilse
      Serial.println(basilantus);
      girilen = girilen + basilantus;
    }
    a++;
  }
  if (sifre == girilen) {
    Serial.println("*********************ŞİFRE DOĞRU GİRİLDİ-**********************");
  }
}

void mesafeSensor(bool kontrol) {
  mesafe = mesafesensor.measureDistanceCm();
  Serial.println(mesafe);
  delay(1000);
  if (mesafe < esikMesafe) {
    bool kontrol = true;
    return kontrol;
  }
  if (mesafe > esikMesafe) {
    bool kontrol = false;
    return kontrol;
  }
}

void insanTespit(bool captcha) {

  bool kameradan_gelen_sonuc;

  if (kameradan_gelen_sonuc == true) {
    bool captcha = true;
    return captcha;
  }
  if (kameradan_gelen_sonuc == false) {
    bool captcha = false;
    return captcha;
  }
}


void gazSensor(bool testSonuc) {
  deger = analogRead(A0);  //Sensörden analog değer okundu

  if (deger > esikDegeri) {  // alkol testi olumsuz sonuçlandı
    bool testSonuc = false;
    return testSonuc;
  } else {
    bool testSonuc = true;
    return testSonuc;
  }
}


void nemSensor(bool onay) {
  delay(2000);
  float nem = dht.readHumidity();
  float sicaklik = dht.readTemperature();  // Sıcaklık (santigrat)
  delay(2000);

  if (nem > 30 && sicaklik > 34) {
    //devam et onay aldın
    onay = true;
    return onay;

  } else {
    //devam edemez insan değil
    onay = false;
    return onay;
  }
}