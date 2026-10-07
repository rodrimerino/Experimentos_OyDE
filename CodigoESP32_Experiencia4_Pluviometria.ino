// Codigo para medicion de precipitacion con pluviometro de cangilones y ESP32.
//
// El pluviometro genera un pulso cada vez que el cangilon se vuelca.
// El programa cuenta la cantidad total de vuelcos y calcula la precipitacion
// acumulada usando una constante de calibracion C definida por el usuario.
//
// Esquema de conexion tipico:
//
// ESP32-S3        Pluviometro
// --------        -----------
// GPIO 4  ------- Señal
// GND     ------- GND
//
// Nota:
// Se usa INPUT_PULLUP, por lo que se asume que el contacto del pluviometro
// cierra contra GND cuando ocurre un vuelco.
// En ese caso, el pulso se detecta en el flanco de bajada: FALLING.


// ================= CONFIGURACION =================

// Pin donde se conecta la señal del pluviometro
const int PIN_PLUVIOMETRO = 14;

// Intervalo entre salidas por Serial, en segundos
const float INTERVALO = 1.0;

// Constante de calibracion del pluviometro
// Unidad: mm por vuelco
// Ejemplo: C = 0.25 significa que cada vuelco equivale a 0.2 mm de 
// precipitacion equivalente
const float C = 0.25;

// Tiempo minimo entre vuelcos validos, en milisegundos
// Sirve para evitar rebotes mecanicos del contacto.
// Para pluviometros de cangilones suele ser razonable usar 100 a 300 ms.
const unsigned long TIEMPO_ANTIRREBOTE = 200;

// =================================================


// Variables modificadas dentro de la interrupcion
volatile unsigned long numeroVuelcos = 0;
volatile unsigned long ultimoPulso = 0;

// Control de tiempo para la salida serial
unsigned long tiempoAnterior = 0;


// Funcion que se ejecuta cada vez que se detecta un pulso
void IRAM_ATTR contarVuelco() {
  unsigned long tiempoActual = millis();

  // Antirrebote: solo cuenta el pulso si paso suficiente tiempo
  // desde el ultimo pulso valido
  if (tiempoActual - ultimoPulso > TIEMPO_ANTIRREBOTE) {
    numeroVuelcos++;
    ultimoPulso = tiempoActual;
  }
}


void setup() {
  Serial.begin(115200);       // Inicializa comunicacion serial
  delay(1000);                // Espera breve para estabilizar la conexion serial

  // Configura el pin del pluviometro con resistencia pull-up interna
  pinMode(PIN_PLUVIOMETRO, INPUT_PULLUP);

  // Asocia una interrupcion al pin del pluviometro
  // FALLING detecta cuando la señal pasa de HIGH a LOW
  attachInterrupt(digitalPinToInterrupt(PIN_PLUVIOMETRO), contarVuelco, FALLING);

  // Encabezado CSV
  Serial.println("Tiempo (s), Nro_vuelcos, PP_acumulada_mm");
}


void loop() {
  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoAnterior >= INTERVALO * 1000) {
    tiempoAnterior = tiempoActual;

    // Copia segura del contador
    // Se desactivan brevemente las interrupciones para evitar leer
    // la variable justo mientras esta siendo modificada.
    noInterrupts();
    unsigned long vuelcos = numeroVuelcos;
    interrupts();

    // Calcula precipitacion acumulada
    float ppAcumulada = vuelcos * C;

    // Tiempo en segundos
    float tiempo_s = tiempoActual / 1000.0;

    // Salida CSV
    Serial.print(tiempo_s, 2);
    Serial.print(", ");
    Serial.print(vuelcos);
    Serial.print(", ");
    Serial.println(ppAcumulada, 3);
  }
}