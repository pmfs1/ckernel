#!/bin/sh
sudo apt-get update -y && sudo apt-get install software-properties-common -y --install-recommends libc6-dev-i386 gcc-multilib g++-multilib genisoimage && make -f Makefile clean && make -f Makefile ckernel && make -f Makefile ckernel vmdk && make -f Makefile ckernel iso

# chmod +x build.sh && ./build.sh