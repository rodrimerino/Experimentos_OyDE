# Experiencias de laboratorio sobre instrumentación meteorológica de bajo costo

Este repositorio contiene los códigos y materiales complementarios correspondientes al trabajo **“¿Qué mide realmente un sensor? Experiencias de laboratorio sobre instrumentación meteorológica de bajo costo”**.

Las actividades fueron desarrolladas para la asignatura **Observación y Diseño Experimental**, de la Licenciatura en Ciencias de la Atmósfera de la Universidad de Buenos Aires. Su objetivo es introducir conceptos fundamentales de la observación meteorológica mediante experiencias de laboratorio basadas en placas ESP32, sensores de bajo costo y materiales de fácil acceso.

## Experiencias incluidas

### 1. Tiempo de respuesta en termometría

Archivo: `CodigoESP32_Experiencia1_TRespuesta.ino`

El programa registra la temperatura medida por una termocupla tipo K conectada a la placa ESP32 mediante un módulo MAX6675. La salida incluye el tiempo transcurrido y la temperatura, con el propósito de analizar la respuesta del instrumento ante un cambio repentino de las condiciones térmicas.

### 2. Psicrometría

Archivo: `CodigoESP32_Experiencia2_Psicrometria.ino`

El programa registra simultáneamente las temperaturas medidas por dos sensores DS18B20 conectados en paralelo mediante el protocolo 1-Wire. Uno de los sensores se utiliza como termómetro de bulbo seco y el otro como termómetro de bulbo húmedo. El código permite incorporar una corrección aditiva para compensar diferencias sistemáticas entre ambos sensores.

### 3. Respuesta espectral de un sensor de iluminancia

Archivo: `CodigoESP32_Experiencia3_RespuestaEspectral.ino`

El programa registra la iluminancia medida por un sensor BH1750 conectado mediante comunicación I²C. Las mediciones se realizan utilizando fuentes LED de diferentes colores para comparar la respuesta del sensor en distintas regiones del espectro.

### 4. Mediciones pluviométricas

Archivo: `CodigoESP32_Experiencia4_Pluviometria.ino`

El programa registra los pulsos generados por un pluviómetro de cangilones y calcula la precipitación acumulada a partir de un factor de calibración definido por el usuario. Se incluye un intervalo antirrebote para evitar el registro de pulsos múltiples durante un mismo movimiento del mecanismo.

## Requisitos

Los programas fueron desarrollados para una placa **ESP32-S3** mediante el entorno de programación Arduino IDE. Para utilizarlos se requiere instalar el soporte para placas ESP32 y, según la experiencia, las siguientes bibliotecas:

- `MAX6675`, para la lectura de la termocupla tipo K.
- `OneWire`, para la comunicación con los sensores DS18B20.
- `DallasTemperature`, para la lectura de los sensores DS18B20.
- `BH1750`, para la lectura del sensor de iluminancia.
- `Wire`, incluida en el entorno Arduino, para la comunicación I²C.

## Utilización general

1. Descargar o clonar este repositorio.
2. Abrir en Arduino IDE el archivo correspondiente a la experiencia.
3. Instalar las bibliotecas requeridas.
4. Conectar los componentes siguiendo el esquema indicado en el código y en el artículo.
5. Revisar los pines, el intervalo de muestreo y las constantes de calibración definidas al comienzo del programa.
6. Seleccionar la placa y el puerto correspondientes.
7. Cargar el programa en la ESP32.
8. Abrir el monitor serial a **115200 baudios** para visualizar y guardar las mediciones.

Los datos se transmiten en un formato separado por comas, compatible con su posterior almacenamiento y procesamiento mediante planillas de cálculo o lenguajes como R y Python.

## Material complementario

El repositorio también incluye —o incorporará progresivamente— las guías de laboratorio, los esquemas de conexión, ejemplos de datos y otros recursos necesarios para reproducir y adaptar las experiencias.

Los códigos pueden requerir modificaciones en los pines de conexión, los intervalos de muestreo o las constantes de calibración según la placa, los sensores y el instrumental utilizados. Estas experiencias fueron diseñadas con fines educativos y no reemplazan los procedimientos de calibración y control de calidad necesarios para efectuar observaciones meteorológicas operativas o de referencia.

## Artículo asociado

----

## Autores y contacto

Rodrigo Andres Merino (rmerino@at.fcen.uba.ar)
Mauro Covi 
Natalia Edith Tonti
María Isabel Gassmann

## Licencia

Los códigos fuente incluidos en este repositorio se distribuyen bajo la licencia MIT. Las guías de laboratorio, figuras, textos y demás materiales didácticos se distribuyen bajo la licencia Creative Commons Atribución 4.0 Internacional (CC BY 4.0), salvo que se indique lo contrario.
Estas licencias permiten utilizar, modificar y redistribuir los materiales, siempre que se reconozca adecuadamente su autoría y se conserven los avisos de licencia correspondientes.
