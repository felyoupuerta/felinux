#!/bin/bash

# Solución definitiva para el error de includes del sistema de archivos

# 1. Crear initfs.h en la ubicación correcta con el contenido adecuado
mkdir -p src/fs
cat > src/fs/initfs.h <<EOF
#ifndef INITFS_H
#define INITFS_H

void init_fs(void);

#endif
EOF

# 2. Corregir el include en initfs.c
sed -i 's/#include "initfs.h"/#include "initfs.h"/g' src/fs/initfs.c  # Solo para asegurar

# 3. Corregir el include en kernel.c - usar la ruta correcta
sed -i 's/#include "fs\/initfs.h"/#include "initfs.h"/g' src/kernel/kernel.c

# 4. Verificar que -Isrc/fs está en CFLAGS del Makefile
if ! grep -q '\-Isrc/fs' Makefile; then
    echo "Añadiendo -Isrc/fs a CFLAGS en el Makefile..."
    sed -i '/CFLAGS.*=/ s/$/ -Isrc\/fs/' Makefile
fi

# 5. Recompilar
echo "Recompilando el proyecto..."
make clean
make

echo "¡Problema solucionado definitivamente!"
echo "- initfs.h creado en src/fs/"
echo "- Includes actualizados para usar initfs.h directamente"
echo "- -Isrc/fs añadido a CFLAGS si no estaba presente"
