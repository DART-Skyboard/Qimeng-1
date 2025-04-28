#!/bin/sh
cd sdcard
make clean && make
cd ../movetomem
cp ../sdcard/sdcard.bin ./
cd move
make clean && make