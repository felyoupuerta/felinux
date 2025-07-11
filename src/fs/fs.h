#ifndef FS_H
#define FS_H

#include <stdint.h>

#define MAX_FILENAME 32
#define MAX_INODES 64

#define FILE_TYPE 0
#define DIR_TYPE 1
#define SYMLINK_TYPE 2

#define FLAG_NONE 0
#define FLAG_HIDDEN 1

void fs_init(void);
void fs_ls(void);
void fs_touch(const char *name);
void fs_cat(const char *name);
extern int current_dir_inode;
void fs_go(const char* path);
const char* fs_get_current_dir_name();
void fs_mk(const char *name);
void fs_nest(const char *name); // Crea y entra en un subdirectorio
void fs_cloak(const char *name); // Marca como oculto
void fs_peek(void); // Muestra la estructura en árbol
void fs_symlink(const char *target, const char *linkname); // Crea un enlace simbólico
void init_fs();
extern int prev_dir_inode;
#endif
