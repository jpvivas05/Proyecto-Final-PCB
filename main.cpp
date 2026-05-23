#include <Arduino.h>

// ===== Pines =====
const int pinSensor = 32; //Entrada analógica
// LEDs de alarma
const int pinAlarma = 21;
const int pinBajo = 19;
const int pinBien = 18;

// ===== Umbrales mV =====
const int umbralAlto = 170;
const int umbralBajo = 150;

// ===== Variables contadoras =====
int contadorEventos = 0;
bool estadoVariante = false;

// ===== Tiempo de medición =====
unsigned long tiempoInicio = 0;
const unsigned long ventanaTiempo = 5000; // 5 segundos

void setup() {

    Serial.begin(115200);

    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);

    pinMode(pinAlarma, OUTPUT);
    pinMode(pinBajo, OUTPUT);
    pinMode(pinBien, OUTPUT);

    digitalWrite(pinAlarma, LOW);
    digitalWrite(pinBajo, LOW);
    digitalWrite(pinBien, LOW);

    tiempoInicio = millis(); //Conteo para medir ritmo respiratorio

    Serial.println("Iniciando lectura...");
}

void loop() {

    // ===== Promedio ADC para estabilidad =====
    int suma = 0;

    for(int i = 0; i < 10; i++){
        suma += analogReadMilliVolts(pinSensor);
        delay(5);
    }

    int voltaje = suma / 10;

    Serial.print("Voltaje: ");
    Serial.println(voltaje);

    // ===== Detección de respiración =====

    if (voltaje > umbralAlto && !estadoVariante) {
        estadoVariante = true;
    }

    if (voltaje < umbralBajo && estadoVariante) {

        contadorEventos++;
        estadoVariante = false;

        Serial.print("Respiracion detectada: ");
        Serial.println(contadorEventos);
    }

    // ===== Evaluar cada ventana de tiempo =====

    if (millis() - tiempoInicio >= ventanaTiempo) {

        // Apagar todos primero
        digitalWrite(pinAlarma, LOW);
        digitalWrite(pinBajo, LOW);
        digitalWrite(pinBien, LOW);

        Serial.print("Total respiraciones: ");
        Serial.println(contadorEventos);

        // ===== Clasificación =====

        if (contadorEventos <= 2) {

            // Respiración baja
            digitalWrite(pinBajo, HIGH);

            Serial.println("RESPIRACION BAJA");

        } else if (contadorEventos <= 6) {

            // Respiración normal
            digitalWrite(pinBien, HIGH);

            Serial.println("RESPIRACION NORMAL");

        } else {

            // Hiperventilación
            digitalWrite(pinAlarma, HIGH);

            Serial.println("HIPERVENTILACION");
        }

        // Reiniciar ventana
        contadorEventos = 0;
        tiempoInicio = millis();
    }

    delay(20);
}
