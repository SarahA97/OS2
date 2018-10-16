CC		:= arm-none-eabi-gcc
AS		:= arm-none-eabi-gcc
LD		:= arm-none-eabi-gcc
OBJCOPY		:= arm-none-eabi-objcopy

CFLAGS		:= -Wall -Wextra -mcpu=arm1176jzf-s -ffreestanding -std=gnu99 -fpic -Os
ASFLAGS		:= -mcpu=arm1176jzf-s -ffreestanding -fpic
LDFLAGS		:= -ffreestanding -nostdlib

MAKEFLAGS	:= -s --no-print-directory

kernel-o	= boot.o uart.o main.o

all: kernel kernel.img

%.o: %.c
	echo -e "  CC      $@"
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	echo -e "  AS      $@"
	$(CC) $(ASFLAGS) -c $< -o $@

kernel: link.ld $(kernel-o)
	echo -e "  LD      $@"
	$(LD) $(LDFLAGS) -T link.ld -o kernel $(kernel-o)

kernel.img: kernel
	echo -e "  OBJCPY  $@"
	$(OBJCOPY) $< -O binary kernel.img

clean:
	rm -f boot.o uart.o main.o
	rm -f kernel kernel.img
