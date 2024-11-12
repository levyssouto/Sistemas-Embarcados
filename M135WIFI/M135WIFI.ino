#include <WiFi.h>
#include <ThingSpeak.h>
#include "MQ135.h"

// Configurações do WiFi e ThingSpeak
const char* nomeRedeWiFi = "SOUSA_2.4GHz";         
const char* senhaWiFi = "53899835";         
WiFiClient clienteWiFi;

unsigned long idCanal = 2729447;         
const char* chaveEscritaAPI = "N18GSRXFKCBKNO3U"; 

const int pinoSensor = 34;  
MQ135 sensorGas(pinoSensor);

void configurar() {
    Serial.begin(9600);
    delay(10);

    // Conectar ao WiFi
    Serial.println("Conectando ao WiFi...");
    WiFi.begin(nomeRedeWiFi, senhaWiFi);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi conectado!");

    // Inicializar ThingSpeak
    ThingSpeak.begin(clienteWiFi);
}

void loop() {
    // Leitura da qualidade do ar pelo sensor MQ135
    int qualidadeAr = sensorGas.getPPM();
    Serial.print("Qualidade do Ar (PPM): ");
    Serial.println(qualidadeAr);

    // Enviar dados para o ThingSpeak
    if (WiFi.status() == WL_CONNECTED) {
        ThingSpeak.setField(1, qualidadeAr);  // Envia o valor para o campo 1

        int codigoResposta = ThingSpeak.writeFields(idCanal, chaveEscritaAPI);
        if (codigoResposta == 200) {
            Serial.println("Dados enviados com sucesso para o ThingSpeak.");
        } else {
            Serial.println("Erro ao enviar dados: Código de resposta " + String(codigoResposta));
        }
    } else {
        Serial.println("Falha na conexão WiFi. Tentando reconectar...");
        WiFi.begin(nomeRedeWiFi, senhaWiFi);
    }

    // Intervalo de 15 segundos
    delay(15000);
}
