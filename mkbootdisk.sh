#!/bin/sh
mkfs rd0
mkdir /rd
mount rd0 /rd
mkdir /rd/boot
mkdir /rd/bin
mkdir /rd/dev
mkdir /rd/proc
mkboot -d /rd -b /boot/boot -l /boot/osldr.dll -k /boot/krnl.dll
cp /boot/os.dll /rd/boot/
cp /bin/sh.exe /rd/bin/
umount /rd
genvmdk /dev/rd0 /var/rd.vmdk
