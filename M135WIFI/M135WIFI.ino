#include <WiFi.h>
#include <ThingSpeak.h>
#include "MQ135.h"

// Configurações do WiFi e ThingSpeak
const char* ssid = "SOUSA_2.4GHz";         
const char* password = "53899835";         
WiFiClient client;

unsigned long channelID = 2729447;         
const char* writeAPIKey = "N18GSRXFKCBKNO3U"; 


const int sensorPin = 34;  
MQ135 gasSensor(sensorPin);

void setup() {
    Serial.begin(9600);
    delay(10);

    // Conectar ao WiFi
    Serial.println("Conectando ao WiFi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi conectado!");

    // Inicializar ThingSpeak
    ThingSpeak.begin(client);
}

void loop() {
    // Leitura da qualidade do ar pelo sensor MQ135
    int air_quality = gasSensor.getPPM();
    Serial.print("Qualidade do Ar (PPM): ");
    Serial.println(air_quality);

    // Enviar dados para o ThingSpeak
    if (WiFi.status() == WL_CONNECTED) {
        ThingSpeak.setField(1, air_quality);  // Envia o valor  para o campo 1

        int responseCode = ThingSpeak.writeFields(channelID, writeAPIKey);
        if (responseCode == 200) {
            Serial.println("Dados enviados com sucesso para o ThingSpeak.");
        } else {
            Serial.println("Erro ao enviar dados: Código de resposta " + String(responseCode));
        }
    } else {
        Serial.println("Falha na conexão WiFi. Tentando reconectar...");
        WiFi.begin(ssid, password);
    }

    // Intervalo  de 15 segundos 
    delay(15000);
}
