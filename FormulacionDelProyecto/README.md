# Formulación del proyecto

En esta carpeta, se dispone todo documento que haga parte del diseño y formulación inicial del proyecto. Se incluye:

## Motivación del proyecto.
Con el fin de afianzar los conocimientos obtenidos a lo largo de la carrera de ingeniería electrónica, se propone como proyecto para el curso de diseño de sistemas electrónicos el diseño de una fuente DC conmutada para uso en laboratorio. Este proyecto incluye temáticas de control, electrónica de potencia, microcontroladores, instrumentación entre otros, haciéndolo ideal como proyecto para la asignatura. 

### Relacion con objetivos ODS.
Este proyecto esta relacionado con el objetivo 7 de los ODS: energía asequible y no contaminante. Mediante el proceso de prototipado se busca entender mas a fondo el proceso de transformación de una entrada AC en una salida DC controlable y así poder entender que componentes y topologías aportan a una mayor eficiencia energética.

## Requerimientos y restricciones:
A continuation se exponen los requerimientos y restricciones del proyecto.

### Eléctricos:
#### Requerimientos:
* El equipo ha de tener una salida de voltaje con rango mínimo de 0-30 V.
* El equipo ha de tener una salida de corriente con rango mínimo de de 0-3A.
* El equipo ha de tener una potencia de salida de 90 W como mínimo.
* La resolución de la medición del voltaje y corriente deben de ser al menos de 10 mV y 1 mA.
#### Restricciones:
* Entrada de voltaje de 60 Hz y 120 V rms.
* El rizado del equipo no puede ser mayor a 50 mV.
* Al cambiar la carga de la fuente el cambio en el voltaje debe de ser menor al 2%.
* El equipo no debe de presentar fallas por interferencias electromagnéticas en un entorno de laboratorio.

### Mecánicos:
#### Requerimientos:
* Un estudiante de universidad promedio debe de ser capaz de transportar el equipo cómodamente.
* La carcasa del equipo debe de ser lo suficientemente robusta para soportar el peso y proteger el interior.
#### Restricciones:
* El equipo no debe de pesar mas de 6 kg.
* El equipo ha de tener dimensiones menores a 40x30x35 cm.

### Térmicas:
#### Requerimientos: 
* El equipo debe de funcionar de manera adecuada a un rango de temperatura entre 0 y 25 grados centígrados.
#### Restricciones:
* La temperatura al interior del equipo no puede superar los 70 grados centígrados.

### Seguridad:
#### Requerimientos:
* El equipo debe de tener conexión a tierra física, correspondiente a un aislamiento para equipos de clase I.
* El equipo debe de tener protecciones contra sobre-tensiones y sobre-corrientes.
* La entrada y la salida deben de estar aisladas.
#### Restricciones:
* La interacción del usuario con los elementos energizados del circuito debe ser reducida al mínimo.
* El usuario no puede acceder al interior del equipo sin el uso de herramientas.

### Interacción con el usuario.
#### Requerimientos:
* El usuario debe de poder modicar los valores de salida desde una interfaz intuitiva.
* El fusible debe de ser accesible para su cambio sin necesidad de desmontar la fuente.
* El usuario debe de poder instalar el equipo de manera rápida en su banco de trabajo.
* Las conexiones necesarias para las entradas deben de ser estándar y disponibles con facilidad.

## Manejo de datos:
Debido a la naturaleza del proyecto, el equipo no trata ni almacena ningún dato del usuario.
* Costos.
* Riesgos.

## Costos:
Se establece como limite de presupuesto inicial 800,000$ pesos colombianos.
#### V1_0

* Diagrama de bloques.
* Interfaces generales.
* Sub-bloques necesarios.
* Consideraciones de etapas intermedias.