#include "commands.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../fs/fs.h"
#include "../lib/string.h"
#include "../include/system.h"


void handle_command(const char* buffer) {
    // Ignora comandos vacíos
    if (buffer[0] == '\0') {
        print("No se ingresó ningún comando.\n");
        return;
    }

    // Comando "ls"
    if (strcmp(buffer, "ls") == 0) {
        fs_ls();
    }

    // Comando "touch archivo"
    else if (strncmp(buffer, "touch ", 6) == 0) {
        const char* filename = buffer + 6;
        if (filename[0] == '\0') {
            print("Uso: touch <nombre_archivo>\n");
        } else {
            fs_touch(filename);
        }
    }

    // Comando "cat archivo"
    else if (strncmp(buffer, "cat ", 4) == 0) {
        const char* filename = buffer + 4;
        if (filename[0] == '\0') {
            print("Uso: cat <nombre_archivo>\n");
        } else {
            fs_cat(filename);
        }
    }
    else if (strcmp(buffer, "clear") == 0) {
        clear_screen();
    }

    // Comando "mk directorio"
    else if (strncmp(buffer, "mk ", 3) == 0) {
        const char* dirname = buffer + 3;
        if (dirname[0] == '\0') {
            print("Uso: mk <nombre_directorio>\n");
        } else {
            fs_mk(dirname);
        }
    }
    // Comando "nest <dir>"
    else if (strncmp(buffer, "nest ", 5) == 0) {
        const char* dirname = buffer + 5;
        if (dirname[0] == '\0') {
            print("Uso: nest <nombre_directorio>\n");
        } else {
            fs_nest(dirname);
        }
    }
    // Comando "cloak <nombre>"
    else if (strncmp(buffer, "cloak ", 6) == 0) {
        const char* name = buffer + 6;
        if (name[0] == '\0') {
            print("Uso: cloak <nombre>\n");
        } else {
            fs_cloak(name);
        }
    }
    // Comando "peek"
    else if (strcmp(buffer, "peek") == 0) {
        fs_peek();
    }
    // Comando "symlink <destino> <nombre>"
    else if (strncmp(buffer, "symlink ", 8) == 0) {
        const char* args = buffer + 8;
        char target[MAX_FILENAME] = {0}, linkname[MAX_FILENAME] = {0};
        int i = 0, j = 0;
        // Parsear primer argumento
        while (*args && *args != ' ' && i < MAX_FILENAME-1) target[i++] = *args++;
        target[i] = '\0';
        while (*args == ' ') args++;
        // Parsear segundo argumento
        while (*args && *args != ' ' && j < MAX_FILENAME-1) linkname[j++] = *args++;
        linkname[j] = '\0';
        if (target[0] && linkname[0]) {
            fs_symlink(target, linkname);
        } else {
            print("Uso: symlink <destino> <nombre_symlink>\n");
        }
    }
    // Comando "pwd"
    else if (strcmp(buffer, "pwd") == 0) {
        fs_pwd();
    }
    // Comando "go <directorio>"
    else if (strncmp(buffer, "go ", 3) == 0) {
        const char* dirname = buffer + 3;
        if (dirname[0] == '\0') {
            print("Uso: go <nombre_directorio>\n");
        } else {
            fs_go(dirname);
        }
    }
    // Comando desconocido
    else {
        print("Comando no reconocido: ");
        print(buffer);
        print("\nUsa 'ls', 'touch <archivo>' o 'cat <archivo>'\n");
    }
}
