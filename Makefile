<<<<<<< HEAD
# Herramientas
CC = gcc
ASM = nasm
LD = ld
GRUB_MKRESCUE = grub-mkrescue

# Directorios
SRCDIR = src
BUILDDIR = build
ISODIR = isodir

# Verificación de directorios (después de definir SRCDIR)
CHECK_DIRS := $(SRCDIR)/include $(SRCDIR)/drivers $(SRCDIR)/kernel $(SRCDIR)/lib $(SRCDIR)/boot
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
         -I$(SRCDIR)/boot

ASMFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T $(SRCDIR)/boot/linker.ld -nostdlib

# Archivos fuente
KERNEL_SRCS = $(wildcard $(SRCDIR)/kernel/*.c) \
              $(wildcard $(SRCDIR)/drivers/*.c) \
              $(wildcard $(SRCDIR)/lib/*.c)

BOOT_SRC = $(SRCDIR)/boot/boot.asm

# Archivos objeto
OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(KERNEL_SRCS)) \
       $(BUILDDIR)/boot.o

# Reglas
all: $(BUILDDIR) $(BUILDDIR)/kernel.bin felinux.iso

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

$(BUILDDIR)/kernel.bin: $(OBJS)
	@echo "[LD] Enlazando kernel..."
	@$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(BUILDDIR)/boot.o: $(BOOT_SRC)
	@echo "[ASM] Ensamblando bootloader..."
	@$(ASM) $(ASMFLAGS) $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@echo "[CC] Compilando $<..."
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $< -o $@

$(ISODIR)/boot/kernel.bin: $(BUILDDIR)/kernel.bin
	@echo "[ISO] Preparando estructura ISO..."
	@mkdir -p $(ISODIR)/boot/grub
	@cp $< $(ISODIR)/boot/
	@echo 'menuentry "Felinux" { multiboot /boot/kernel.bin }' > $(ISODIR)/boot/grub/grub.cfg

felinux.iso: $(ISODIR)/boot/kernel.bin
	@echo "[GRUB] Generando imagen ISO..."
	@$(GRUB_MKRESCUE) -o $@ $(ISODIR) 2>/dev/null
	@echo "ISO generada: felinux.iso"

run: felinux.iso
	@echo "[QEMU] Iniciando emulación..."
	@qemu-system-i386 -cdrom felinux.iso -no-reboot -no-shutdown -d cpu_reset

clean:
	@echo "[CLEAN] Limpiando proyecto..."
	@rm -rf $(BUILDDIR) $(ISODIR) felinux.iso

.PHONY: all clean run
=======
CC = i686-linux-gnu-gcc
LD = i686-linux-gnu-ld
CFLAGS = -ffreestanding -nostdlib -Wall -Wextra -Iinclude -std=gnu99 -m32
LDFLAGS = -T linker.ld -nostdlib -m elf_i386

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,build/%.o,$(SRC))

.PHONY: all clean iso run

all: kernel.bin

kernel.bin: $(OBJ)
	$(LD) $(LDFLAGS) -o build/kernel.bin $(OBJ)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/*.o build/kernel.bin minios.iso isodir

iso: kernel.bin
	mkdir -p isodir/boot/grub
	cp build/kernel.bin isodir/boot/
	cp boot/grub/grub.cfg isodir/boot/grub/
	grub-mkrescue -o minios.iso isodir

run: iso
	qemu-system-i386 -cdrom minios.iso
>>>>>>> origin/main
