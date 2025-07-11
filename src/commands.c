#include "fs.h"
#include "../drivers/screen.h"
#include "../lib/string.h"

void execute_command(char *input) {
    char *command = strtok(input, " ");
    char *arg = strtok(NULL, " ");
    
    if (strcmp(command, "ls") == 0) {
        fs_ls();
    } 
    else if (strcmp(command, "touch") == 0 && arg != NULL) {
        fs_touch(arg);
    } 
    else if (strcmp(command, "mk") == 0 && arg != NULL) {
        fs_mk(arg);
    } 
    else if (strcmp(command, "go") == 0 && arg != NULL) {
        fs_go(arg);
    } 
    else if (strcmp(command, "pwd") == 0) {
        fs_pwd();
    } 
    else if (strcmp(command, "cat") == 0 && arg != NULL) {
        fs_cat(arg);
    }
    else if (strcmp(command, "nest") == 0 && arg != NULL) {
        fs_nest(arg);
    }
    else if (strcmp(command, "cloak") == 0 && arg != NULL) {
        fs_cloak(arg);
    }
    else if (strcmp(command, "peek") == 0) {
        fs_peek();
    }
    else if (strcmp(command, "symlink") == 0) {
        char *target = arg;
        char *linkname = strtok(NULL, " ");
        if (target && linkname) {
            fs_symlink(target, linkname);
        } else {
            print("Uso: symlink <destino> <nombre_symlink>\n");
        }
    }
    else {
        print("Comando no reconocido: ");
        print(input);
        print("\nComandos disponibles: ls, touch <archivo>, mk <directorio>, go <directorio>, pwd, cat <archivo>\n");
    }
}

void print_help() {
    print("Comandos disponibles:\n");
    print("ls       - Listar archivos y directorios\n");
    print("touch    - Crear archivo\n");
    print("mk       - Crear directorio (abreviatura de mkdir)\n");
    print("go       - Cambiar de directorio\n");
    print("pwd      - Mostrar directorio actual\n");
    print("cat      - Mostrar contenido de archivo\n");
    print("help     - Mostrar esta ayuda\n");
    print("nest     - Crea y entra en un subdirectorio\n");
    print("cloak    - Marca archivo/directorio como oculto\n");
    print("peek     - Muestra la estructura de archivos en árbol\n");
    print("symlink  - Crea un enlace simbólico\n");
}