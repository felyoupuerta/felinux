# Herramientas
CC = i686-linux-gnu-gcc
ASM = nasm
LD = i686-linux-gnu-ld
GRUB_MKRESCUE = grub-mkrescue

# Directorios
SRCDIR = src
BUILDDIR = build
ISODIR = isodir

# Verificación de carpetas obligatorias
CHECK_DIRS := $(SRCDIR)/include $(SRCDIR)/drivers $(SRCDIR)/kernel $(SRCDIR)/lib $(SRCDIR)/boot $(SRCDIR)/fs
$(foreach dir,$(CHECK_DIRS),$(if $(wildcard $(dir)),,$(error Directory $(dir) missing)))

# Flags de compilación
CFLAGS = -m32 \
-ffreestanding \
	 -nostdlib \
	 -nostdinc \
	 -fno-builtin \
	 -fno-stack-protector \
	 -nostartfiles \
	 -nodefaultlibs \
	 -Wall \
	 -Wextra \
	 -fno-pic \
	 -c \
	 -I$(SRCDIR)/include \
	 -I$(SRCDIR)/drivers \
	 -I$(SRCDIR)/kernel \
	 -I$(SRCDIR)/lib \
	 -I$(SRCDIR)/fs \
	 -I$(SRCDIR)/boot

ASMFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T $(SRCDIR)/boot/linker.ld -nostdlib

# Archivos fuente
KERNEL_SRCS = $(wildcard $(SRCDIR)/kernel/*.c) \
	      $(wildcard $(SRCDIR)/drivers/*.c) \
	      $(wildcard $(SRCDIR)/lib/*.c) \
	      $(wildcard $(SRCDIR)/fs/*.c)

BOOT_SRC = $(SRCDIR)/boot/boot.asm

# Archivos objeto
OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(KERNEL_SRCS)) \
       $(BUILDDIR)/boot.o

# Reglas principales
all: $(BUILDDIR) $(BUILDDIR)/kernel.bin felinux.iso

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

build/fs/initfs.o: src/fs/initfs.c | build/fs/
	$(CC) $(CFLAGS) -c src/fs/initfs.c -o build/fs/initfs.o
# Compilación del kernel
$(BUILDDIR)/kernel.bin: $(OBJS)
	@echo "[LD] Enlazando kernel..."
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Ensamblaje del bootloader
$(BUILDDIR)/boot.o: $(BOOT_SRC)
	@echo "[ASM] Ensamblando bootloader..."
	$(ASM) $(ASMFLAGS) $< -o $@

# Compilación de archivos C
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@echo "[CC] Compilando $<..."
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Preparar directorio ISO
$(ISODIR)/boot/kernel.bin: $(BUILDDIR)/kernel.bin
	@echo "[ISO] Preparando estructura ISO..."
	mkdir -p $(ISODIR)/boot/grub
	cp $< $(ISODIR)/boot/
	echo 'menuentry "Felinux" { multiboot /boot/kernel.bin }' > $(ISODIR)/boot/grub/grub.cfg

# Crear ISO booteable
felinux.iso: $(ISODIR)/boot/kernel.bin
	@echo "[GRUB] Generando imagen ISO..."
	$(GRUB_MKRESCUE) -o $@ $(ISODIR) 2>/dev/null
	@echo "ISO generada: felinux.iso"

# Ejecutar QEMU
run: felinux.iso
	@echo "[QEMU] Iniciando emulación..."
	qemu-system-i386 -cdrom felinux.iso -no-reboot -no-shutdown -d cpu_reset -curses



# Limpiar proyecto
clean:
	@echo "[CLEAN] Limpiando proyecto..."
	rm -rf $(BUILDDIR) $(ISODIR) felinux.iso

.PHONY: all clean run
