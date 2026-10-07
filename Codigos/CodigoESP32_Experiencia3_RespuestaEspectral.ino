// Codigo para medicion de luminosidad con sensor BH1750 y ESP32.

// Esquema de conexion
//
// Componentes:
// 1 sensor BH1750
//
// Conexion tipica I2C:
// ESP32-S3        BH1750
// ---------       -------
// 3.3V  --------- VCC
// GND   --------- GND
// PIN8  --------- SDA
// PIN9  --------- SCL
//

#include <Wire.h>     // Libreria para comunicacion I2C
#include <BH1750.h>   // Libreria para el sensor BH1750

// ================= CONFIGURACION =================
const float INTERVALO = 1.0;      // Intervalo entre mediciones en segundos

// Si queres fijar manualmente los pines I2C, descomenta estas lineas:
// #define SDA_PIN 8
// #define SCL_PIN 9

// =================================================

// Objeto del sensor
BH1750 lightMeter;

// Control de tiempo
unsigned long tiempoAnterior = 0; // Guarda el tiempo de la ultima medicion (ms desde inicio)

void setup() {
  Serial.begin(115200);           // Inicializa comunicacion serial a 115200 baudios
  delay(1000);                    // Espera breve para estabilizar la conexion serial

  // Inicializar I2C
  // Si necesitas pines manuales, usar:
  // Wire.begin(SDA_PIN, SCL_PIN);
  Wire.begin();

  // Inicializar sensor
  if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("Error: no se pudo inicializar el sensor BH1750");
    while (true);                 // Detiene el programa si el sensor no responde
  }

  // Encabezado
  Serial.println("Tiempo (s), Lux"); // Encabezado CSV
}

void loop() {
  unsigned long tiempoActual = millis(); // Tiempo actual desde el inicio del programa en ms

  if (tiempoActual - tiempoAnterior >= INTERVALO * 1000) {
    tiempoAnterior = tiempoActual; // Actualiza tiempo de referencia

    // Leer iluminancia en lux
    float lux = lightMeter.readLightLevel();


    // Tiempo en segundos
    float tiempo_s = tiempoActual / 1000.0;

    // Salida CSV
    Serial.print(tiempo_s, 2);    // Tiempo
    Serial.print(", ");
    Serial.println(lux, 2);  // Lux corregidos
  }
}