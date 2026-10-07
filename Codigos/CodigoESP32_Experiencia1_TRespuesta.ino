// Programa para estudiar el tiempo de respuesta de una termocupla.

// Conexión ESP32 S3 – MAX6675

// Conexiones:
// MAX6675	  ESP32 S3
// VCC	      3.3V
// GND	      GND
// SCK	      GPIO 18
// CS       	GPIO 5
// SO       	GPIO 19
// (El MAX6675 usa SPI "por software", así que se pueden elegir pines, cambiándolos en el código.)

// Conexión de la termocupla en el MAX6675:
// Verificar la polaridad (+ y -). Tanto en el MAX6675 como en el conector de la termocupla están indicadas.

#include <MAX6675.h>     // Librería para manejar el módulo MAX6675 (lectura de termocupla tipo K). El nombre es case sensitive, dependiendo del nombre de la Libreria, ver si va en mayúscula o minúscula.
#include <math.h>        // Librería matemática estándar (necesaria para usar isnan())

// ---------------- CONFIGURACIÓN ----------------
const float INTERVALO_SEGUNDOS = 0.25;   
// Intervalo de muestreo en segundos (tipo float)
// Puede modificarse manualmente antes de cargar el programa
// Se convierte luego a milisegundos para comparación con millis()

// Pines (modificables según conexión física)
int pinSO  = 19;  
// Pin de datos de salida del MAX6675 (SO = Serial Out)
// Tipo: entero (GPIO del ESP32 S3)

int pinCS  = 5;   
// Pin Chip Select (CS) del MAX6675
// Controla cuándo el módulo envía datos

int pinSCK = 18;  
// Pin de reloj (CLK o SCK) del MAX6675
// Sincroniza la comunicación SPI

// Creación del objeto termocupla
MAX6675 termocupla(pinCS, pinSO, pinSCK);
// Se instancia el objeto con:
// - pinSCK: reloj
// - pinCS: selección de chip
// - pinSO: datos
// Este objeto permite llamar a funciones como readCelsius()

// Variables de tiempo
unsigned long tiempoAnterior = 0;  
// Guarda el último instante en que se realizó una medición (en ms)
// Tipo unsigned long: necesario para millis()

unsigned long tiempoInicio = 0;    
// Guarda el tiempo inicial (cuando arranca el programa)
// Se usa para calcular tiempo relativo

unsigned long intervalo_ms = INTERVALO_SEGUNDOS * 1000;  
// Conversión de segundos a milisegundos
// millis() trabaja en ms, por eso se escala
// Tipo: unsigned long (aunque proviene de float)

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);  
  // Inicializa comunicación serial
  // Parámetro: velocidad en baudios (115200 recomendado para ESP32)

  delay(1000);  
  // Pausa de 1000 ms
  // Permite estabilizar el sistema y el MAX6675 antes de comenzar


  termocupla.begin();

  tiempoInicio = millis();  
  // Guarda el tiempo inicial del sistema (en ms desde encendido)
  // millis() devuelve unsigned long

  Serial.println("Tiempo (s), T");  
  // Imprime encabezado CSV
  // println agrega salto de línea automático
}

// ---------------- LOOP PRINCIPAL ----------------
void loop() {

  unsigned long tiempoActual = millis();  
  // Lee el tiempo actual del sistema (ms desde arranque)

  if (tiempoActual - tiempoAnterior >= intervalo_ms) {  
    // Condición de muestreo:
    // Ejecuta solo si pasó el intervalo definido
    // Evita usar delay() → permite ejecución no bloqueante

    tiempoAnterior = tiempoActual;  
    // Actualiza el último tiempo de medición
    // Fundamental para mantener periodicidad

    float tiempo_s = (tiempoActual - tiempoInicio) / 1000.0;  
    // Calcula tiempo relativo en segundos
    // - resta tiempo inicial
    // - divide por 1000 para pasar de ms a s
    // Tipo float para permitir decimales

    // Hacer lectura explícita
    uint8_t estado = termocupla.read();

    float temperatura = termocupla.getCelsius();  
    // Lee la temperatura desde el MAX6675
    // Devuelve float en °C
    // Puede devolver NaN si hay error o termocupla desconectada

    if (isnan(temperatura)) {  
      // Verifica si el valor es "Not a Number"
      // Función isnan() de math.h

      temperatura = -99.999;  
      // Reemplaza valor inválido por código de faltante
      // Mantiene formato float para compatibilidad con CSV
    }

    Serial.print(tiempo_s, 2);  
    // Imprime tiempo en segundos
    // Parámetros:
    // - variable (tiempo_s)
    // - número de decimales (1)

    Serial.print(",");  
    // Imprime separador CSV (coma)

    Serial.println(temperatura, 3);  
    // Imprime temperatura
    // Parámetros:
    // - variable (temperatura)
    // - número de decimales (3)
    // println agrega salto de línea
  }
}
