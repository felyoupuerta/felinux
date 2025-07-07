#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "commands.h"

void kernel_main() {
    clear_screen();
    print("Bienvenido a Felinux!\n> ");

    char buffer[128];
    while (1) {
        read_input(buffer, sizeof(buffer));
        handle_command(buffer);
        print("\n> ");
    }
}
