# Planteamiento inicial del proyecto. 

---


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

## Diagramas de bloques

Diagrama de bloques general V1.0:

![Diagrama de bloques](FormulacionDelProyecto/V1_0_DiagramaDeBloques/Imgs/Bloques2.png)
## Control analógico y digital

Se deberá tener un control mixto. El control digital se encargara de:

* Setpoints, manejo interfaz usuario e intermediación de ser necesario.

Para la parte de control analógico, se encargara del sensado y ejecución de los lazos. Se propone un diagrama de bloques general para control y sensado general:

Se selecciona como microcontrolador el PIC16F17556, principalmente debido al ADC con una resolución de 12 bits, adicionalmente los 28 pins que posee el controlador permite conectar el teclado matricial, pantalla y botones de encendido y selección de la modalidad.



