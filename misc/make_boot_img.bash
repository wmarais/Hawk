#!/bin/bash

EFI_APP=$1
EFI_IMG=$2

# Create a disk image.
dd if=/dev/zero of=${EFI_IMG} bs=512 count=93750

# Create a partion table and an EFI partition.
parted ${EFI_IMG} -s -a minimal mklabel gpt
parted ${EFI_IMG} -s -a minimal mkpart EFI FAT16 2048s 93716s
parted ${EFI_IMG} -s -a minimal toggle 1 boot

# Create a temporary EFI partion
dd if=/dev/zero of=/tmp/part.img bs=512 count=91669
mformat -i /tmp/part.img -h 32 -t 32 -n 64 -c 1

# Copy the UEFI application to the root directory of the partition.
mcopy -i /tmp/part.img ${EFI_APP} ::/

# Write the partition image into the main disk image.
dd if=/tmp/part.img of=${EFI_IMG} bs=512 count=91668 seek=2048 conv=notrunc

# Start the emulator with the disk image.
qemu-system-x86_64 -cpu qemu64 -bios /usr/share/qemu/OVMF.fd -drive file=${EFI_IMG},format=raw,if=ide -net none

#qemu-system-x86_64 -drive format=raw,file=${EFI_IMG},if=ide


#-cpu qemu64 -bios /usr/share/qemu/OVMF.fd
