CC = ~/Documents/Arch64CC/gcc-arm-10.2-2020.11-x86_64-aarch64-none-elf/bin/aarch64-none-elf-gcc
CC_FLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
AS = ~/Documents/Arch64CC/gcc-arm-10.2-2020.11-x86_64-aarch64-none-elf/bin/aarch64-none-elf-as
LINK = ~/Documents/Arch64CC/gcc-arm-10.2-2020.11-x86_64-aarch64-none-elf/bin/aarch64-none-elf-ld
OBJCPY = ~/Documents/Arch64CC/gcc-arm-10.2-2020.11-x86_64-aarch64-none-elf/bin/aarch64-none-elf-objcopy
GDB = ~/Documents/Arch64CC/gcc-arm-10.2-2020.11-x86_64-aarch64-none-elf/bin/aarch64-none-elf-gdb
HEADERS = $(wildcard *.h)
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: run

boot.o: boot.S
	${CC} ${CC_FLAGS} -c $^ -o $@

%.o: %.c
	${CC} ${CC_FLAGS} -c $< -o $@

kernel8.img: boot.o $(OBJS)
	$(LINK) -nostdlib -nostartfiles $^ -T link.ld -o kernel8.elf
	$(OBJCPY) -O binary kernel8.elf kernel8.img

run: kernel8.img
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial stdio

test.o: test.s
	${AS} $^ -o $@ -g

test: test.o
	${CC} $^ -o $@ -nostdlib -static

testRun: test 
	qemu-aarch64 ./test

testDebug: test
	${GDB} ./$@

clean:
	rm *.o *.elf *.img
