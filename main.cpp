#include <Arduino.h>

// ===============================
// Pines
// ===============================
const int pinSensor = 32;

const int ledRojo  = 21;
const int ledAzul  = 19;
const int ledVerde = 18;

// ===============================
// Configuracion ADC
// ===============================
const float Vref = 3.3;

void setup() {

    Serial.begin(115200);

    // ADC ESP32
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);

    // LEDs
    pinMode(ledRojo, OUTPUT);
    pinMode(ledAzul, OUTPUT);
    pinMode(ledVerde, OUTPUT);

    // Encender todos los LEDs
    digitalWrite(ledRojo, HIGH);
    digitalWrite(ledAzul, HIGH);
    digitalWrite(ledVerde, HIGH);

    Serial.println("Prueba de hardware iniciada");
}

void loop() {

    // Promedio para estabilidad
    int suma = 0;

    for(int i = 0; i < 10; i++) {
        suma += analogReadMilliVolts(pinSensor);
        delay(5);
    }

    int voltaje = suma / 10;

    // Mostrar en monitor serial
    Serial.print("Voltaje sensor: ");
    Serial.print(voltaje);
    Serial.println(" mV");

    delay(500);
}
