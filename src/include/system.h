#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

// Funciones de control del sistema
void cli(void);    // Deshabilita interrupciones
void sti(void);    // Habilita interrupciones
void halt(void);   // Detiene la CPU

// Funciones de utilidad
void reboot(void); // Reinicio del sistema (opcional)

#endif
