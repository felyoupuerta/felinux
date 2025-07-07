#!/bin/bash

# Corrector de includes para system.h en proyecto Felinux

# Ruta base del proyecto
PROJECT_DIR="$(pwd)"
SYS_HEADER="system.h"
CORRECT_PATH="include/system.h"

# Buscar todos los archivos .c y .h que incluyan system.h
files_to_check=$(grep -rl "#include .*system.h" "$PROJECT_DIR/src")

# Contadores
total_files=0
corrected_files=0

echo "=== Verificando includes de system.h ==="

for file in $files_to_check; do
    ((total_files++))
    
    # Verificar si el include es correcto (usando <system.h>)
    if grep -q "#include\s*[\"<]$SYS_HEADER[\">]" "$file"; then
        # Es correcto si usa <> y está en include/
        if grep -q "#include\s*<$SYS_HEADER>" "$file"; then
            echo "[OK] $file - Include correcto"
        else
            # Corregir a #include <system.h>
            sed -i 's/#include\s*["'"'"']\(.*\)'"'"'"system.h["'"'"']/#include <system.h>/g' "$file"
            ((corrected_files++))
            echo "[FIXED] $file - Corregido a #include <system.h>"
        fi
    fi
done

echo "=== Resumen ==="
echo "Archivos verificados: $total_files"
echo "Archivos corregidos: $corrected_files"
echo "Proceso completado."

# Verificar archivos que deberían incluir system.h pero no lo hacen
echo ""
echo "=== Archivos que podrían necesitar system.h ==="
find "$PROJECT_DIR/src" -type f \( -name "*.c" -o -name "*.h" \) ! -exec grep -q "system.h" {} \; -print | while read file; do
    if grep -q "cli\|sti\|halt" "$file"; then
        echo "ADVERTENCIA: $file usa funciones de system.h pero no lo incluye"
    fi
done
