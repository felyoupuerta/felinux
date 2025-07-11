#include "../drivers/screen.h"
#include "../lib/string.h"
#include "initfs.h"

void init_fs() {
    print("Montando sistema de archivos...\n");
    // Crear estructura de carpetas estándar
    fs_mk("bin");
    fs_mk("dev");
    fs_mk("etc");
    fs_mk("home");
    fs_mk("tmp");
    fs_mk("root");
    fs_mk("usr");
    fs_mk("var");
}
