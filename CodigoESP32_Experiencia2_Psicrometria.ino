// Codigo para psicrómetro basado en sensores DS18B20.

// Esquema de conexión

// Componentes:
// 2 sensores DS18B20
// 1 resistencia 4.7 kΩ

// Conexión (modo típico, ambos en paralelo)
// ESP32-S3        DS18B20 (x2)
// ---------       -------------
// 3.3V  --------- VDD (en general, cable rojo).
// GND   --------- GND (en general, cable negro).
// GPIO4 --------- DATA (en general, cable amarillo) Conectar ambos sensores.
// Entre DATA y 3.3V:
// Puentear con una resistencia de 4.7 kΩ (pull-up)

#include <OneWire.h>              // Librería para comunicación OneWire (protocolo usado por DS18B20)
#include <DallasTemperature.h>   // Librería de alto nivel para manejar sensores DS18B20

// ================= CONFIGURACIÓN =================
#define ONE_WIRE_BUS 4           // Define el pin GPIO 4 del ESP32-S3 como bus de datos OneWire (puede cambiarse)

const float INTERVALO = 2.0;     // Intervalo entre mediciones en segundos (tipo float permite valores no enteros, pero no utilizar valores menores a 1 por el tiempo de medición de los sensores DS18B20).

// ---- Corrección de sensor ----
const float OFFSET_TIW = 0.0;    // Corrección aditiva en °C para Tiw (ej: +0.3 o -0.2)
// =================================================

// Objeto OneWire
OneWire oneWire(ONE_WIRE_BUS);   // Crea un objeto "oneWire" asociado al pin definido (requiere número de pin)

// Objeto DallasTemperature
DallasTemperature sensors(&oneWire); // Crea objeto "sensors" y le pasa referencia al bus OneWire (& = dirección de memoria)

// Direcciones de los sensores
DeviceAddress sensor1, sensor2;  // Arrays de 8 bytes donde se almacenan las direcciones únicas de cada DS18B20

// Control de tiempo
unsigned long tiempoAnterior = 0; // Variable para guardar el tiempo de la última medición (en ms desde inicio)

void setup() {
  Serial.begin(115200);          // Inicializa comunicación serial a 115200 baudios (velocidad de transmisión)
  delay(1000);                   // Espera 1 segundo para estabilizar el monitor serial

  sensors.begin();               // Inicializa la librería DallasTemperature (detecta sensores en el bus)

  // Detectar sensores
  if (sensors.getDeviceCount() < 2) { // getDeviceCount(): devuelve cantidad de sensores detectados en el bus
    Serial.println("Error: se necesitan 2 sensores DS18B20"); // Mensaje de error si hay menos de 2 sensores
    while (true);                // Bucle infinito: detiene el programa si no se cumple condición
  }

  // Obtener direcciones
  sensors.getAddress(sensor1, 0); // Guarda en "sensor1" la dirección del sensor en índice 0 del bus
  sensors.getAddress(sensor2, 1); // Guarda en "sensor2" la dirección del sensor en índice 1 del bus

  // Resolución (opcional)
  sensors.setResolution(sensor1, 12); // Configura resolución del sensor1 en 12 bits (máxima precisión)
  sensors.setResolution(sensor2, 12); // Configura resolución del sensor2 en 12 bits

  // Encabezado
  Serial.println("Tiempo (s), T, Tiw"); // Imprime encabezado CSV
}

void loop() {
  unsigned long tiempoActual = millis(); // Obtiene tiempo actual desde inicio del programa en milisegundos

  if (tiempoActual - tiempoAnterior >= INTERVALO * 1000) {
    tiempoAnterior = tiempoActual; // Actualiza el tiempo de referencia

    sensors.requestTemperatures(); // Dispara conversión simultánea en todos los sensores

    float T   = sensors.getTempC(sensor1); // Temperatura sin corregir del sensor 1
    float Tiw = sensors.getTempC(sensor2); // Temperatura sin corregir del sensor 2

    // Aplicar corrección
    float Tiw_corr = Tiw + OFFSET_TIW; // Suma el offset definido (puede ser positivo o negativo)

    float tiempo_s = tiempoActual / 1000.0; // Convierte tiempo a segundos

    // Salida CSV
    Serial.print(tiempo_s, 2);   // Tiempo
    Serial.print(", ");
    Serial.print(T, 2);          // Sensor 1 (sin corrección)
    Serial.print(", ");
    Serial.println(Tiw_corr, 2); // Sensor 2 corregido
  }
}
