# Consideraciones adicionales al elegir amplificador operacional

### Requerimientos generales

Un amplificador operacional seleccionado como candidato “global” debería cumplir, en la medida de lo posible, con las siguientes características:

- **Entrada y salida rail-to-rail**, para maximizar el rango dinámico con alimentación simple  
- **Bajo offset de entrada**, especialmente importante en etapas de sensado  
- **Baja deriva térmica**, para mantener estabilidad en el tiempo  
- **Ancho de banda suficiente**, acorde a la frecuencia del lazo de control  
- **Bajo ruido**, especialmente en etapas de medición  
- **Estabilidad en ganancia unitaria**, para uso como buffer  
- **Capacidad de operación con la alimentación disponible** (por ejemplo, 3.3 V o 5 V)
### Compromisos de diseño

Aunque el uso de un único amplificador operacional es conveniente, en la práctica implica ciertos compromisos:

- Un amplificador optimizado para **precisión (bajo offset)** puede ser más costoso o tener menor ancho de banda  
- Un amplificador optimizado para **velocidad** puede presentar mayor ruido o offset  
- Algunas etapas críticas (por ejemplo, sensado de corriente de alta precisión) pueden requerir dispositivos especializados  

Por esta razón, es común adoptar una estrategia híbrida:

- Un amplificador operacional “general” para buffers, divisores y etapas no críticas  
- Dispositivos específicos para funciones críticas (por ejemplo, amplificadores de sensado de corriente)

### Recomendación de implementación

Se recomienda:

- Seleccionar un amplificador operacional principal que cubra la mayoría de los bloques del sistema  
- Verificar que cumple con los requisitos más exigentes dentro del diseño  
- Evaluar si alguna etapa específica requiere un dispositivo distinto  

Esta estrategia permite mantener un balance adecuado entre costo, simplicidad y desempeño del sistema.

# Conclusiones sobre sección. 

