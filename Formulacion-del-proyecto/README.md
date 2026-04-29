# Formulación del proyecto

En esta carpeta, se dispone todo documento que haga parte del diseño y formulación inicial del proyecto. Se incluye:

## Motivación del proyecto.
Con el fin de afianzar los conocimientos obtenidos a lo largo de la carrera de ingeniería electrónica, se propone como proyecto para el curso de diseño de sistemas electrónicos el diseño de una fuente DC conmutada para uso en laboratorio. Este proyecto incluye temáticas de control, electrónica de potencia, microcontroladores, instrumentación entre otros, haciéndolo ideal como proyecto para la asignatura. 

### Relacion con objetivos ODS.
Este proyecto esta relacionado con el objetivo 7 de los ODS: energía asequible y no contaminante. Mediante el proceso de prototipado se busca entender mas a fondo el proceso de transformación de una entrada AC en una salida DC controlable y así poder entender que componentes y topologías aportan a una mayor eficiencia energética.

# Requerimientos y restricciones

A continuación se exponen los requerimientos y restricciones del proyecto.

## 1. Eléctricos

### Requerimientos

- **E1:** El equipo ha de tener una salida de voltaje con rango mínimo de 0-30 V.
- **E2:** El equipo ha de tener una salida de corriente con rango mínimo de 0-3 A.
- **E3:** El equipo ha de tener una potencia de salida de 90 W como mínimo.
- **E4:** La resolución de la medición del voltaje y corriente deben ser de al menos 10 mV y 5 mA.
- **E5:** El rizado del equipo no puede ser mayor a 50 mV.
- **E6:** Al cambiar la carga de la fuente, el cambio en el voltaje debe ser menor al 2%.
- **E7:** El equipo debe poder operar en modalidad CC y VC.
- **E8:** La eficiencia de potencia debe ser de al menos 50%.

### Restricciones

- **E9:** Entrada de voltaje de 60 Hz y 120 V rms, con un 10% de margen de error.
- **E10:** El equipo no debe presentar fallas por interferencias electromagnéticas en un entorno de laboratorio.

## 2. Mecánicos

### Requerimientos

- **M1:** El equipo no debe pesar más de 6 kg.
- **M2:** El equipo ha de tener dimensiones menores a 30 × 15 × 35 cm.

### Restricciones

- **M3:** El equipo debe tener zonas de agarre previstas para un transporte más fácil.
- **M4:** La carcasa debe soportar el peso total del equipo y conservar su integridad estructural durante manipulación y transporte normal, sin deformaciones permanentes, apertura accidental ni exposición de componentes internos.

## 3. Térmicas

### Restricciones

- **T1:** La temperatura al interior del equipo no puede superar los 70 °C.
- **T2:** El equipo debe funcionar de manera adecuada en un rango de temperatura ambiente entre 0 y 25 °C.

## 4. Seguridad

### Restricciones

- **S1:** La interacción del usuario con los elementos energizados del circuito debe ser reducida al mínimo.
- **S2:** El usuario no puede acceder al interior del equipo sin el uso de herramientas.
- **S3:** El equipo debe tener conexión a tierra física, correspondiente a un aislamiento para equipos de clase I.
- **S4:** El equipo debe tener protecciones contra sobretensiones y sobrecorrientes.
- **S5:** La entrada y la salida deben estar aisladas.
- **S6:** La fuente debe soportar cortocircuito en la salida sin daño permanente.

## 5. Interacción con el usuario

### Requerimientos

- **I1:** La salida debe poder apagarse y encenderse desde la interfaz.
- **I2:** El usuario debe poder modificar los valores de salida desde una interfaz.
- **I3:** La interfaz debe mostrar las variables de corriente y voltaje en tiempo real.
- **I4:** El fusible debe ser accesible para su cambio sin necesidad de desmontar la fuente.
- **I5:** El usuario debe poder instalar el equipo de manera rápida en su banco de trabajo.
- **I6:** La medición mostrada en pantalla debe corresponder con los valores reales de las salidas con un error menor a 5 mV.
- **I7:** La interfaz de usuario ha de ser intuitiva; un estudiante de ingeniería debe poder entender cómo usarla después de revisar el manual.

### Restricciones

- **I8:** Las conexiones necesarias para las entradas deben ser estándar y disponibles con facilidad.
## Manejo de datos:
Debido a la naturaleza del proyecto, el equipo no trata ni almacena ningún dato del usuario.
* Costos.
* Riesgos.

## Costos:
Se establece como limite de presupuesto inicial 800,000$ pesos colombianos.

## Consideraciones adicionales

En primer lugar, es necesario tener las siguientes consideraciones adicionales al diagrama de bloques general:

1. Fuentes auxiliares para alimentar circuitos digitales y opamps.
2. Aislamiento de lazo de control via optoacopladores.
3. Driver para MOSFET o driver general de topología. 
4. Interfaces ADC y DAC.
5. Protección en salida contra corto.
6. Compensadores de lazo.
7. Bleeder en salida.
8. Low Rout.

### Diagramas de bloques

Diagrama de bloques general de la fuente a diseñar:

![Diagrama de bloques](https://github.com/juamorenogo/DCSource_DIY/blob/main/Formulacion-del-proyecto/Imgs/DiagramaBloques.png)
### Control analógico y digital

Se deberá tener un control mixto. El control digital se encargara de:

* Setpoints, manejo interfaz usuario e intermediación de ser necesario.