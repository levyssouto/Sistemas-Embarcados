#include "BluetoothSerial.h"

const int smokeSensor = 14;   // Pino do sensor de fumaça
const int ledPin = 2;        
String device_name = "ESP32-BT-Slave";

// verifica se o Bluetooth está disponível
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// verifica se o Serial Port Profile está disponível
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(9600);              // inicia  monitor serial
  SerialBT.begin(device_name);     // inicia bluetooh com nome do dispositivo
  pinMode(ledPin, OUTPUT);         
  Serial.printf("O dispositivo \"%s\" foi iniciado.\nAgora você pode emparelhá-lo com o Bluetooth!\n", device_name.c_str());
}

void loop() {
  int digitalNumber = analogRead(smokeSensor);  // vê os valores do sensor de fumaça
  Serial.println(digitalNumber);                
  SerialBT.println(digitalNumber);              // envia os valores por Bluetooth para o celular

  // se o valor lido for maior que 1000, o LED pisca
  if (digitalNumber > 1000) {
    digitalWrite(ledPin, HIGH);  
    delay(500);                  
    digitalWrite(ledPin, LOW);   
    delay(500);                  
  }

  delay(1000); // atraso de 1 segundo entre leituras para evitar sobrecarga
}

