# Hawk
C++ based operating system.

```
sudo apt-get install gnu-efi qemu-system-x86
```

```
sudo qemu-system-x86_64 -cpu qemu64 \
  -drive if=pflash,format=raw,unit=0,file=/usr/share/OVMF/OVMF_CODE.fd,readonly=on \
  -drive if=pflash,format=raw,unit=1,file=/usr/share/OVMF/OVMF_VARS.fd \
  -net none
```


