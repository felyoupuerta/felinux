#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "commands.h"
#include "../fs/fs.h"    
#include "../lib/string.h"
#include "initfs.h"

void kernel_main() {
    clear_screen();
    print("Bienvenido a Felinux!\n");
    init_fs();
    fs_init();
    // Volver a home tras crear estructura
    fs_go("home");

    print("Sistema de archivos inicializado.\n");
    print("Usa 'ls', 'touch archivo', 'cat archivo', 'go <dir>', 'pwd'...\n");

    char buffer[128];
    while (1) {
        print("\n");
        print(fs_get_current_dir_name());
        print(" > ");
        read_input(buffer, sizeof(buffer));
        handle_command(buffer);
    }
}