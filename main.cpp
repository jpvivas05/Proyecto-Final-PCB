#include <Arduino.h>
// Configuracion de sensor de respiracion
const int pinSensor = 32;
const int pinAlarma = 21;
const int pinBajo = 19;
const int pinBien = 18;

const float Vref = 3.3;
const int umbralAlto = 170;
const int umbralBajo = 150;

int contadorEventos = 0;
int eventosNecesarios =5;
bool estadoVariante = false;

bool ledEncendido = false;
unsigned long tiempoInicioLED = 0;

const unsigned long tiempoEncendido = 3000;

void setup() {
    Serial.begin(115200);

    // Resolucion ADC
    analogReadResolution(12);
    //Medicion hasta 3.3
    analogSetAttenuation(ADC_11db);
    pinMode(pinAlarma, OUTPUT);
    pinMode(pinBajo, OUTPUT);
    pinMode(pinBien, OUTPUT);

    delay(100);
    Serial.println("Iniciando lectura...");
}

void loop() {
  //Promedio para estabilidad
    int suma = 0;

  for(int i = 0; i < 10; i++){
    suma += analogReadMilliVolts(pinSensor);
    delay(5);
  }

  int voltaje = suma / 10;

  Serial.print("Voltaje: ");
  Serial.print(voltaje);
  Serial.println(" mV");

  if (!ledEncendido){
    if (voltaje>umbralAlto && !estadoVariante){
      estadoVariante = true;
    }
    if (voltaje<umbralBajo && estadoVariante){
      contadorEventos++;
      estadoVariante = false;

      Serial.print("Evento: ");
      Serial.println(contadorEventos);
    }
    if (contadorEventos>= eventosNecesarios){
      digitalWrite(pinAlarma, HIGH);
      delay(5);
      digitalWrite(pinBajo, HIGH);
      delay(5);
      digitalWrite(pinBien, HIGH);
      delay(5);
      ledEncendido = true;
      tiempoInicioLED = millis();
      Serial.println("LED Encendido, ALARMA");
    }
  }
  if (ledEncendido){
    if(millis() - tiempoInicioLED>= tiempoEncendido){
      digitalWrite(pinAlarma, LOW);
      digitalWrite(pinBajo, LOW);
      digitalWrite(pinBien, LOW);
      ledEncendido = false;
      contadorEventos = 0;
      estadoVariante = false;
      Serial.println("LED Apagado, contador reiniciado");
      delay(10);
    }
  }
}
}
