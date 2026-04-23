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
* E1: El equipo ha de tener una salida de voltaje con rango mínimo de 0-30 V.
* E2: El equipo ha de tener una salida de corriente con rango mínimo de de 0-3A.
* E3: El equipo ha de tener una potencia de salida de 90 W como mínimo.
* E4: La resolución de la medición del voltaje y corriente deben de ser al menos de 10 mV y 5 mA.
* E5: El rizado del equipo no puede ser mayor a 50 mV.
* E6: Al cambiar la carga de la fuente el cambio en el voltaje debe de ser menor al 2%.
* E7: El equipo debe poder operar en modalidad CC y VC.
* E8: Eficiencia de potencia de al menos 50%.


#### Restricciones:
* E9: Entrada de voltaje de 60 Hz y 120 V rms, con un 10% de margen de error.
* E10: El equipo no debe presentar fallas por interferencias electromagnéticas en un entorno de laboratorio.


### Mecánicos:
#### Requerimientos:
* M1: El equipo no debe pesar más de 6 kg.
* M2: El equipo ha de tener dimensiones menores a 40x30x35 cm.

#### Restricciones:
* M3: El equipo debe de tener zonas de agarre previstas para un transporte más fácil.
* M4: La carcasa debe soportar el peso total del equipo y conservar su integridad estructural durante manipulación y transporte normal, sin deformaciones permanentes, apertura accidental ni exposición de componentes internos. 

### Térmicas:
#### Restricciones:
* T1: La temperatura al interior del equipo no puede superar los 70 grados centígrados.
* T2: El equipo debe de funcionar de manera adecuada a un rango de temperatura ambiente entre 0 y 25 grados centígrados.

### Seguridad:
#### Restricciones:
* S1: La interacción del usuario con los elementos energizados del circuito debe ser reducida al mínimo.
* S2: El usuario no puede acceder al interior del equipo sin el uso de herramientas.
* S3: El equipo debe de tener conexión a tierra física, correspondiente a un aislamiento para equipos de clase I.
* S4: El equipo debe de tener protecciones contra sobretensiones y sobrecorrientes.
* S5: La entrada y la salida deben de estar aisladas.
* S6: La fuente debe soportar cortocircuito en la salida sin daño permanente. 


### Interacción con el usuario.
#### Requerimientos:
* I1: La salida debe de poder apagarse y encenderse desde la interfaz.
* I2: El usuario debe de poder modificar los valores de salida desde una interfaz.
* I3: La interfaz debe mostrar las variables de corriente y voltaje en tiempo real.
* I4: El fusible debe de ser accesible para su cambio sin necesidad de desmontar la fuente.
* I5: El usuario debe de poder instalar el equipo de manera rápida en su banco de trabajo.
* I6: La medición mostrada en pantalla debe corresponder con los valores reales de las salidas con un error menor a 5 mV. 
* I7: La interfaz de usuario ha de ser intuitiva, un estudiante de ingeniería debe de poder entender el cómo usarla después de revisar el manual.


#### Restricciones:
* I8: Las conexiones necesarias para las entradas deben de ser estándar y disponibles con facilidad.

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