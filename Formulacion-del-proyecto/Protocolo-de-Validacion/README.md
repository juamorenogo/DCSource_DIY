# Propuesta de pruebas combinadas

## P1. Desempeño eléctrico estático

**Requisitos cubiertos:** E1, E2, E4, E7, I3, I6, I8

### Montaje
- Prototipo.
- Banana caimán.
- Resistencias y reóstatos.
- 2 multímetros.
- Computador con spreadsheets.

### Procedimiento
- Se ajustan un total de 20 puntos a lo largo de su rango de 30 V.
- Se ajustan un total de 10 puntos a lo largo de su rango de 3 A.
- Comparar:
  - Valor configurado
  - Valor mostrado en pantalla
  - Valor medido por instrumento externo
- Se verifica en diferentes rangos de incrementos o decrementos de 10 mV y 5 mA.

### Criterio de aceptación
- Salida ajustable al menos entre 0–30 V.
- Corriente ajustable/regulada al menos entre 0–3 A.
- Resolución mínima de 10 mV y 5 mA.
- La lectura en pantalla debe quedar dentro de una tolerancia de 5 mV.

---

## P2. Regulación y calidad de salida

**Requisitos cubiertos:** E4, E5, E6, I3, I8

### Montaje
- Prototipo.
- Banana caimán.
- Carga electrónica.
- Osciloscopio.
- Multímetro.

### Procedimiento
- Medir el rizado de salida en vacío, media carga y carga máxima.
- Hacer escalones de carga, por ejemplo de 10% a 90% de la corriente nominal.
- Ajustar la fuente para forzar operación en CV y luego en CC.
- Observar si la interfaz refleja correctamente el modo y las variables en tiempo real.

### Criterio de aceptación
- Rizado ≤ 50 mV.
- Variación de voltaje ante cambio de carga < 2%.
- La fuente debe pasar correctamente entre CV y CC.
- La pantalla debe mostrar V e I en tiempo real.

---

## P3. Eficiencia y operación con la red de entrada

**Requisitos cubiertos:** E6, E8, E9, I8

### Montaje
- Prototipo.
- Pinza amperimétrica.
- Multímetro.
- Fuente AC regulable o autotransformador, si el laboratorio lo permite.
- Reóstato.

### Procedimiento
- Ensayar la fuente a 108 V, 120 V y 132 V AC a 60 Hz.
- Medir potencia de entrada y de salida en distintos puntos de carga.

### Criterio de aceptación
- Eficiencia ≥ 50%.
- Operación correcta en todo el rango de entrada especificado.
- Conectores/cableado de entrada estándar y de fácil disponibilidad.

---

## P4. Seguridad eléctrica y protecciones

**Requisitos cubiertos:** S1, S3, S4, S5, S6, E10

### Montaje
- Inspección física.
- Multímetro / comprobador de continuidad.
- Bananas caimán.
- Entorno de laboratorio con equipos típicos conectados.

### Procedimiento
- Revisar que no haya acceso accidental a partes energizadas.
- Verificar continuidad de tierra física.
- Confirmar existencia y actuación de fusible/protecciones.
- Comprobar aislamiento entre entrada y salida.
- Aplicar un cortocircuito controlado a la salida y verificar recuperación.
- Operar la fuente en entorno de laboratorio con otras cargas y equipos para comprobar su funcionamiento sin mínimas alteraciones.

### Criterio de aceptación
- Tierra física presente y funcional.
- Protecciones instaladas y operativas.
- Entrada y salida aisladas.
- El cortocircuito no debe causar daño permanente.
- No hay alteraciones en el comportamiento del prototipo.

---

## P5. Verificación térmica

**Requisitos cubiertos:** T1, T2, E3, E8

### Montaje
- Termopares o termómetro infrarrojo.
- Carga electrónica de alta potencia, probablemente reóstato.
- Medición de temperatura ambiente.

### Procedimiento
- Operar la fuente durante 5 minutos cercano a los 90 W de potencia.
- Medir temperatura en disipadores, componentes de potencia y aire interno.

### Criterio de aceptación
- Temperatura interna < 70 °C.
- Funcionamiento correcto entre 0 y 25 °C ambiente.

---

## P6. Verificación mecánica y de portabilidad

**Requisitos cubiertos:** M1, M2

### Materiales
- Prototipo.
- Báscula.
- Cinta métrica.

### Procedimiento
- Medir peso y dimensiones máximas.
- Hacer una inspección de la carcasa y puntos de fijación.

### Criterio de aceptación
- Peso ≤ 6 kg.
- Dimensiones menores a 40 × 30 × 35 cm.

---

## P7. Accesibilidad y usabilidad de la interfaz

**Requisitos cubiertos:** I1, I2, I4, I5, I7, M3, M4

### Materiales
- Prototipo.
- Fusibles.
- Usuarios.
- Encuesta por teléfono.

### Procedimiento
- Dar a un estudiante el manual.
- Pedirle que califique en escala de 1 a 10:
  - Transporte el equipo.
  - Conecte el equipo.
  - Encienda el equipo.
  - Configure un voltaje y un límite de corriente.
  - Active y desactive la salida.
  - Identifique el fusible y explique cómo cambiarlo.
- Se verifica el estado de la carcasa en búsqueda de deformaciones en la carcasa o daños.
- Registrar tiempo, errores y necesidad de ayuda.

### Criterio de aceptación
- El usuario completa las tareas sin ayuda relevante tras leer el manual.
- El fusible es accesible sin desarmar el equipo.
- La interfaz resulta comprensible e intuitiva.
- Transporte cómodo por usuario promedio.
- Instalación rápida en banco.
- Carcasa adecuada para manipulación y transporte normal.
- Las encuestas obtienen una calificación promedio superior a 7.