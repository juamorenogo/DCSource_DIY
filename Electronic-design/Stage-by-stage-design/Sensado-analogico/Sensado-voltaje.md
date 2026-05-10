# Medición de voltaje

El sensado de voltaje presenta una **complejidad significativamente menor en comparación con el sensado de corriente**, ya que la magnitud a medir se encuentra directamente disponible y en un rango suficientemente alto (hasta 30 V en este caso). Esto elimina la necesidad de etapas de amplificación de alta ganancia para señales de muy baja amplitud.
## Implementación básica para lectura digital

La forma más común de realizar el sensado es mediante un divisor resistivo, el cual escala el voltaje de salida a un rango adecuado para el ADC:

$$
V_{adc} = V_{out} \cdot \frac{R_2}{R_1 + R_2}
$$

Para un sistema con ADC de 3.3 V y un rango de salida de hasta 30 V, se puede emplear una relación aproximada de 10:1.

### Ejemplo

- $R_1 = 90\,k\Omega$
- $R_2 = 10\,k\Omega$

Lo que produce:

$$
30\,V \rightarrow 3\,V
$$

### Uso de buffer

Es recomendable añadir un amplificador operacional en configuración de seguidor de voltaje (buffer) a la salida del divisor resistivo, con los siguientes objetivos:

- Aislar el divisor del ADC  
- Evitar errores por carga del ADC  
- Mejorar la estabilidad de la medición  

## Implementación básica para lectura analógica

Para el lazo de control analógico, el voltaje sensado debe cumplir:

- Ser proporcional al voltaje real de salida  
- Ser estable en el tiempo  
- Presentar bajo nivel de ruido  

El mismo divisor resistivo puede ser utilizado como señal de realimentación. Sin embargo, es recomendable el uso de un buffer para evitar interacción entre el lazo de control y otras etapas del circuito.

En este contexto, no es necesaria una precisión absoluta extrema, sino una relación consistente entre la señal sensada y el voltaje real.
## Consideraciones de diseño

### Precisión del divisor

- La relación $R_1 / R_2$ define directamente la exactitud de la medición  
- Se recomienda el uso de resistencias con tolerancia ≤ 0.1%  
- Idealmente, resistencias con bajo TCR (≤ 50 ppm/°C)  

### Deriva térmica

- Cambios de temperatura afectan el valor de las resistencias  
- Introducen un error proporcional al voltaje medido  

### Resolución del ADC

Para un ADC de 12 bits y 3.3 V:

$$
LSB \approx \frac{3.3V}{4096} \approx 0.8\,mV
$$

Esto equivale aproximadamente a:

$$
0.8\,mV \cdot 10 = 8\,mV \text{ en la salida}
$$

Lo cual se encuentra cercano al requerimiento de 10 mV de resolución.

### Filtrado

Se recomienda añadir un filtro RC en la entrada del ADC para:

- Reducir ruido de conmutación  
- Mejorar la estabilidad de la medición  

Ejemplo típico:

- Resistencia serie: 100 Ω – 1 kΩ  
- Capacitor a tierra: 10 nF – 100 nF  

### Layout y buenas prácticas

- Mantener el nodo del divisor lo más corto posible  
- Separar rutas de potencia y señal  
- Usar plano de tierra continuo  
- Evitar acoplamiento con señales de conmutación  

### Consideraciones adicionales

- La impedancia del divisor no debe ser excesivamente alta, ya que incrementa la susceptibilidad al ruido  
- Tampoco debe ser muy baja, para evitar consumo innecesario de corriente  
- Valores típicos entre 10 kΩ y 100 kΩ son adecuados  

## Consideraciones al elegir amplificador operacional para medición de voltaje

En el bloque de medición de voltaje, el amplificador operacional se utiliza principalmente como **buffer** a la salida del divisor resistivo. Aunque esta aplicación no es tan exigente como el sensado de corriente, la selección del dispositivo sigue siendo relevante para garantizar estabilidad y precisión adecuadas.
### Función del amplificador en este bloque

El amplificador operacional cumple las siguientes funciones:

- Aislar el divisor resistivo del ADC o del lazo analógico  
- Evitar errores debidos a la carga de la siguiente etapa  
- Mantener la relación de voltaje definida por el divisor  

Dado que se utiliza típicamente en configuración de seguidor de voltaje, la estabilidad en ganancia unitaria es un requisito fundamental.
### Requerimientos principales

Para esta aplicación, el amplificador operacional debe cumplir con:

- **Entrada y salida rail-to-rail**, para permitir operación cercana a los límites de alimentación  
- **Bajo offset de entrada**, para minimizar errores en la medición  
- **Baja corriente de polarización de entrada**, evitando errores adicionales en el divisor resistivo  
- **Estabilidad en ganancia unitaria**, necesaria para operación como buffer  
- **Bajo ruido**, especialmente si el sistema presenta conmutación cercana  
- **Compatibilidad con la alimentación disponible**.
### Influencia del offset

El offset del amplificador introduce un error directo en la medición de voltaje. Sin embargo, a diferencia del sensado de corriente, este efecto es menos crítico debido al mayor nivel de señal.

Por ejemplo, un offset de 1 mV en la entrada del buffer se traduce aproximadamente en:

$$
10\,mV \text{ en la salida real}
$$

(considerando un divisor 10:1)

Esto se encuentra en el mismo orden de magnitud que la resolución requerida, por lo que debe ser tenido en cuenta, aunque no representa una limitación crítica.
### Corriente de entrada

Si la corriente de polarización del amplificador es significativa, puede introducir un error en el nodo del divisor:

- Este efecto es mayor si se utilizan resistencias de alto valor  
- Se recomienda utilizar amplificadores con corrientes de entrada bajas (pA - entrada FET)

### Consideraciones de velocidad

El ancho de banda requerido en este bloque es bajo, ya que:

- La señal de voltaje es relativamente lenta  
- El objetivo es medición, no conmutación rápida  

Por lo tanto, no es necesario un amplificador de alta velocidad, lo que permite priorizar precisión y bajo ruido.

En general, para este bloque, se recomienda utilizar un amplificador operacional de propósito general con buenas características de precisión, el cual puede ser compartido con otros bloques del sistema. **No es necesario utilizar amplificadores especializados**, siempre que se cumplan los requerimientos mencionados.