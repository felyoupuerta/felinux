#include "fs.h"
#include "ramdisk.h"
#include <string.h>
#include "../lib/string.h"
#include "../drivers/screen.h"

int current_dir_inode = 0;
int prev_dir_inode = 0;

typedef struct {
    char name[MAX_FILENAME];
    uint8_t type;
    uint32_t size;
    uint32_t block;
    int parent_inode;
    uint8_t flags; // Flags para oculto, solo lectura, etc.
    int symlink_target; // -1 si no es symlink, si no, apunta al inodo destino
} inode_t;

#define INODE_TABLE_START_BLOCK 2
#define DATA_START_BLOCK 20

static inode_t inodes[MAX_INODES];
static uint32_t next_data_block = DATA_START_BLOCK;

void fs_init() {
    ramdisk_init();
    memset(inodes, 0, sizeof(inodes));

    // Inicializar directorio raíz
    inode_t root = {
        .name = "/",
        .type = DIR_TYPE,
        .size = 0,
        .block = DATA_START_BLOCK,
        .parent_inode = -1,  // El directorio raíz no tiene padre
        .flags = 0,
        .symlink_target = -1
    };
    inodes[0] = root;
}

void fs_ls() {
    print("Contenido del directorio actual:\n");
    for (int i = 0; i < MAX_INODES; i++) {
        if (inodes[i].name[0] != '\0' && inodes[i].parent_inode == current_dir_inode) {
            print(inodes[i].name);
            print(inodes[i].type == FILE_TYPE ? " [FILE]\n" : " [DIR]\n");
        }
    }
}

void fs_touch(const char *name) {
    for (int i = 0; i < MAX_INODES; i++) {
        if (inodes[i].name[0] == '\0') {
            strncpy(inodes[i].name, name, MAX_FILENAME);
            inodes[i].type = FILE_TYPE;
            inodes[i].size = 0;
            inodes[i].block = next_data_block++;
            inodes[i].parent_inode = current_dir_inode;
            inodes[i].flags = 0;
            inodes[i].symlink_target = -1;
            print("Archivo creado: ");
            print(name);
            print("\n");
            return;
        }
    }
    print("Error: no hay espacio en tabla de inodos.\n");
}

void fs_mk(const char *name) {
    // Verificar si ya existe un directorio con ese nombre
    for (int i = 0; i < MAX_INODES; i++) {
        if (strcmp(inodes[i].name, name) == 0 && inodes[i].type == DIR_TYPE && 
            inodes[i].parent_inode == current_dir_inode) {
            print("Error: ya existe un directorio con ese nombre.\n");
            return;
        }
    }

    // Crear nuevo directorio
    for (int i = 0; i < MAX_INODES; i++) {
        if (inodes[i].name[0] == '\0') {
            strncpy(inodes[i].name, name, MAX_FILENAME);
            inodes[i].type = DIR_TYPE;
            inodes[i].size = 0;
            inodes[i].block = next_data_block++;
            inodes[i].parent_inode = current_dir_inode;
            inodes[i].flags = 0;
            inodes[i].symlink_target = -1;
            print("Directorio creado: ");
            print(name);
            print("\n");
            return;
        }
    }
    print("Error: no hay espacio en tabla de inodos.\n");
}

void fs_go(const char *path) {
    if (strcmp(path, "/") == 0) {
        prev_dir_inode = current_dir_inode;
        current_dir_inode = 0;
        print("Directorio cambiado a /\n");
        return;
    }
    if (strcmp(path, "-") == 0) {
        int tmp = current_dir_inode;
        current_dir_inode = prev_dir_inode;
        prev_dir_inode = tmp;
        print("Directorio cambiado a: ");
        print(inodes[current_dir_inode].name);
        print("\n");
        return;
    }
    // Retroceso (go ..)
    if (strcmp(path, "..") == 0) {
        if (inodes[current_dir_inode].parent_inode != -1) {
            prev_dir_inode = current_dir_inode;
            current_dir_inode = inodes[current_dir_inode].parent_inode;
            print("Directorio cambiado a: ");
            print(inodes[current_dir_inode].name);
            print("\n");
        } else {
            print("Ya estás en el directorio raíz.\n");
        }
        return;
    }
    // Buscar el directorio
    for (int i = 0; i < MAX_INODES; i++) {
        if (inodes[i].type == DIR_TYPE && 
            strcmp(inodes[i].name, path) == 0 &&
            inodes[i].parent_inode == current_dir_inode) {
            prev_dir_inode = current_dir_inode;
            current_dir_inode = i;
            print("Directorio cambiado a: ");
            print(inodes[i].name);
            print("\n");
            return;
        }
    }
    print("Error: directorio no encontrado.\n");
}

const char *fs_get_current_dir_name() {
    return inodes[current_dir_inode].name;
}

void build_path(int inode, char *buffer) {
    if (inodes[inode].parent_inode == -1) {
        kstrcpy(buffer, "/");
        return;
    }
    char temp[256] = "";
    build_path(inodes[inode].parent_inode, temp);
    if (strcmp(temp, "/") != 0) kstrcat(temp, "/");
    kstrcat(temp, inodes[inode].name);
    kstrcpy(buffer, temp);
}

void fs_pwd() {
    char path[256];
    build_path(current_dir_inode, path);
    print(path);
    print("\n");
}

void fs_nest(const char *name) {
    fs_mk(name);
    fs_go(name);
}

void fs_cloak(const char *name) {
    for (int i = 0; i < MAX_INODES; i++) {
        if (strcmp(inodes[i].name, name) == 0 && inodes[i].parent_inode == current_dir_inode) {
            inodes[i].flags |= FLAG_HIDDEN;
            print("Oculto: ");
            print(name);
            print("\n");
            return;
        }
    }
    print("Error: archivo/directorio no encontrado.\n");
}

void fs_peek_rec(int inode, int depth) {
    for (int d = 0; d < depth; d++) print("  ");
    print(inodes[inode].name);
    if (inodes[inode].type == DIR_TYPE) print("/\n");
    else if (inodes[inode].type == SYMLINK_TYPE) print(" -> symlink\n");
    else print("\n");
    for (int i = 0; i < MAX_INODES; i++) {
        if (inodes[i].parent_inode == inode && inodes[i].name[0] != '\0' && !(inodes[i].flags & FLAG_HIDDEN)) {
            fs_peek_rec(i, depth + 1);
        }
    }
}

void fs_peek(void) {
    print("Estructura de archivos:\n");
    fs_peek_rec(0, 0);
}

void fs_symlink(const char *target, const char *linkname) {
    int target_inode = -1;
    for (int i = 0; i < MAX_INODES; i++) {
        if (strcmp(inodes[i].name, target) == 0 && inodes[i].parent_inode == current_dir_inode) {
            target_inode = i;
            break;
        }
    }
    if (target_inode == -1) {
        print("Error: destino no encontrado.\n");
        return;
    }
    for (int i = 0; i < MAX_INODES; i++) {
        if (inodes[i].name[0] == '\0') {
            strncpy(inodes[i].name, linkname, MAX_FILENAME);
            inodes[i].type = SYMLINK_TYPE;
            inodes[i].size = 0;
            inodes[i].block = 0;
            inodes[i].parent_inode = current_dir_inode;
            inodes[i].flags = 0;
            inodes[i].symlink_target = target_inode;
            print("Symlink creado: ");
            print(linkname);
            print(" -> ");
            print(target);
            print("\n");
            return;
        }
    }
    print("Error: no hay espacio para symlink.\n");
}

void fs_cat(const char *name) {
    for (int i = 0; i < MAX_INODES; i++) {
        if (strcmp(inodes[i].name, name) == 0 && inodes[i].parent_inode == current_dir_inode && inodes[i].type == FILE_TYPE) {
            print("[Contenido de archivo no implementado]\n");
            return;
        }
    }
    print("Error: archivo no encontrado.\n");
}