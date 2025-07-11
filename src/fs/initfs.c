#include "../drivers/screen.h"
#include "../lib/string.h"
#include "initfs.h"

void init_fs() {
    print("Montando sistema de archivos...\n");

    print("/bin     → comandos ejecutables\n");
    print("/dev     → dispositivos\n");
    print("/etc     → configuraciones\n");
    print("/home    → usuarios\n");
    print("/tmp     → archivos temporales\n");
    print("/root    → root user\n");
    print("/usr     → programas y bibliotecas\n");
    print("/var     → logs y datos variables\n");
}
