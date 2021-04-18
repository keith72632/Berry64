CC = ~/Documents/Arch64CC/gcc-arm-10.2-2020.11-x86_64-aarch64-none-elf/bin/aarch64-none-elf-gcc
CC_FLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
LINK = ~/Documents/Arch64CC/gcc-arm-10.2-2020.11-x86_64-aarch64-none-elf/bin/aarch64-none-elf-ld
OBJCPY = ~/Documents/Arch64CC/gcc-arm-10.2-2020.11-x86_64-aarch64-none-elf/bin/aarch64-none-elf-objcopy
HEADERS = $(wildcard *.h)
all: run

boot.o: boot.S
	${CC} ${CC_FLAGS} -c $^ -o $@

kernel.o: kernel.c
	${CC} ${CC_FLAGS} -c $^ -o $@

uart.o: uart.c
	${CC} ${CC_FLAGS} -c $^ -o $@

mbox.o: mbox.c
	${CC} ${CC_FLAGS} -c $^ -o $@

kernel8.img: boot.o kernel.o uart.o mbox.o
	$(LINK) -nostdlib -nostartfiles $^ -T link.ld -o kernel8.elf
	$(OBJCPY) -O binary kernel8.elf kernel8.img

run: kernel8.img
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio

clean:
	rm *.o *.elf *.img
