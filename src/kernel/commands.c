#include "commands.h"
#include "screen.h"
#include "string.h"
#include <system.h>
void handle_command(const char* buffer) {
    if (strcmp(buffer, "hola") == 0) {
        print("¡Hola Felipe!\n");
    } else if (strcmp(buffer, "ayuda") == 0) {
        print("Comandos disponibles: hola, ayuda, salir\n");
    } else if (strcmp(buffer, "salir") == 0) {
        print("Saliendo del sistema...\n");
        asm volatile("cli");
        asm volatile("hlt");
    } else {
        print("Comando desconocido\n");
    }
}
