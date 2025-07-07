#include "screen.h"
#include "keyboard.h"
#include "memory.h"
#include "commands.h"
#include "multiboot.h"

void kernel_main() {
    clear_screen();
    print("Felinux v0.1\n");

    print("\n\n\n");

    print("Escribe 'help' Para ver los posibles comandos.\n\n");
    
    init_keyboard();
    init_memory();
    
}
