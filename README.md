# Proyecto Generator
Proyecto desarrollado para la EDU-CIAA-NXP que simula tiradas de Generala con dados virtuales. Permite agitar la consola para generar resultados al azar, seleccionar dados a conservar y muestra combinaciones y puntajes en pantalla. Incluye efectos visuales y sonoros, con funcionamiento inalámbrico.

---

## Características Principales
- **Detección de Movimiento:** Implementada con el sensor MPU6050 para detectar la agitación del dispositivo.
- **Pantalla:** Utiliza un display ILI9341 para mostrar los dados, combinaciones y puntajes.
- **Interfaz Táctil:** Incorporada mediante el controlador XPT2046 para interactuar con los dados y configuraciones del juego.
- **Sistema de Audio:** Señales digitales generadas por un DAC de 10 bits, amplificadas con un LM386 y reproducidas en un parlante de 20 mm.
- **Portabilidad:** Alimentación mediante una batería integrada que permite un uso completamente inalámbrico y autónomo del dispositivo.

---

## Instalación
1. **Clonar el repositorio**  
   En una terminal, ejecuta el siguiente comando:
  ```git
  git clone https://github.com/RamiroMadera/Proyecto_GENERATOR.git
  ```
