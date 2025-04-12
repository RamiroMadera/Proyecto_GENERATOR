# GENERATOR 🎲  
**Consola electrónica portátil para jugar a la Generala**  
Dispositivo desarrollado con la **EDU-CIAA-NXP** que emula tiradas de dados, combinaciones y puntajes del tradicional juego de Generala. Integra una pantalla táctil, efectos de sonido, sensores de movimiento y una interfaz gráfica atractiva, todo en un sistema autónomo alimentado por batería.

---

## 🎮 Características principales

- **Simulación de tiradas de Generala**
  - Aleatorización de dados entre 1 y 6.
  - Selección de dados para conservar y volver a tirar.
  - Evaluación automática del puntaje y combinaciones.

- **Pantalla gráfica interactiva**
  - Display TFT 2.8" con controlador **ILI9341**.
  - Interfaz táctil basada en **XPT2046** para seleccionar dados y comenzar partidas.

- **Detección de movimiento**
  - Sensor **MPU6050** para identificar la agitación del dispositivo y gatillar la tirada.

- **Sistema de audio**
  - Generación de sonidos con DAC de 10 bits.
  - Amplificación mediante **LM386**.
  - Efectos que acompañan la agitación, la tirada y la obtención de una Generala.

- **Portabilidad total**
  - Alimentado por una batería externa de **5100 mAh**, con una autonomía estimada de más de **16 horas** de juego continuo.

---

## 🧠 Arquitectura del sistema

- **Microcontrolador:** EDU-CIAA-NXP  
- **Interacción por estados:** Inicio → Reposo → Sacudiendo 
- **Firmware en C** desarrollado bajo **CIAA Launcher** 

---

## 📦 Instalación

1. Clona el repositorio:
   ```bash
   git clone https://github.com/RamiroMadera/Proyecto_GENERATOR.git
   ```
2. Abrí el proyecto con CIAA Launcher.
3. Compila y carga el firmware a la placa.
4. Conecta los periféricos según el [esquemático incluido](https://github.com/RamiroMadera/Proyecto_GENERATOR/blob/main/kicad/GENERATOR_Esquematico.pdf).
5. ¡Listo para jugar!

---

## 📚 Documentación técnica

El proyecto cuenta con documentación completa en el informe final del curso "Taller de Proyecto I" de la Facultad de Ingeniería (UNLP), que incluye:

- Requerimientos de hardware y software
- Diseño esquemático y PCB en KiCad
- Arquitectura del firmware y máquina de estados
- Métodos de aleatorización
- Validación del sistema y videos de prueba

[📄 Informe Final (PDF)](https://github.com/RamiroMadera/Proyecto_GENERATOR/blob/main/docs/InformeFinal-Grupo8-TallerDeProyectoI.pdf)

---

## 🧪 Validación y videos

Se ha validado el correcto funcionamiento de todos los requerimientos. Puedes ver los resultados en esta [playlist de YouTube](https://www.youtube.com/playlist?list=PLVBSSKjP-CQ1Y1-SsgbkpVqvSBivJ-5Vr), que muestra:

- Encendido/apagado  
- Tiradas completas  
- Selección de dados  
- Sonido y efectos especiales  
- Reconocimiento de combinaciones y puntajes

---

## 🛠️ Créditos

**Autores:**  
- Tobias Garcia Iacovelli  
- Ramiro Madera  
- Ulises Pereira  

Este proyecto fue desarrollado como parte del curso **Taller de Proyecto I** en la **Facultad de Ingeniería, Universidad Nacional de La Plata**.

---

## 🚀 Próximos pasos

- Añadir sistema de historial de puntajes  
- Incorporar carcasa protectora  
- Expansión a múltiples juegos de dados  
- Conectividad en red para multijugador o leaderboard en la nube