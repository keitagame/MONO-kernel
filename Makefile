CC = gcc
AS = as
LD = ld

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -nostdlib -fno-stack-protector -fno-pic
LDFLAGS = -T linker.ld -nostdlib -m elf_i386

OBJS = boot.o gdt.o gdt_flush.o idt.o serial.o kernel.o pmm.o process.o switch.o timer.o sys.o userprog.o usermode.o syscall.o tss.o isr_syscall_stub.o

all: kernel.elf iso

boot.o: boot.s
	$(AS) --32 -o $@ $<
isr_syscall_stub.o: isr_syscall_stub.s
	$(AS) --32 -o $@ $<
gdt_flush.o: gdt_flush.s
	$(AS) --32 -o $@ $<

switch.o: switch.s
	$(AS) --32 -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

iso: kernel.elf grub.cfg
	mkdir -p iso/boot/grub
	cp kernel.elf iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o minimal.iso iso

run: all
	qemu-system-i386 -cdrom minimal.iso -nographic -d int,cpu_reset -no-reboot -D qemu.log

clean:
	rm -rf *.o kernel.elf iso minimal.iso
