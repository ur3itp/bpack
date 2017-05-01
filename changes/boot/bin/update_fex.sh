#!/bin/bash

#this scripts appends boot configuration file (see fex) into /boot
#updates boot image.
#if config file is a .fex, then it will be recompiled as well.

if [ -z $1 ] 
then
	printf "Usage:\n\t $(basename $0) <configuration file name>\n"
	exit
fi
if ! [ -f $1 ]
then
	printf "File '$1' doesn't exist\n"
	exit
fi

if [ ${1##*.} == "fex" ]
then
	binName=$(dirname $1)/$(basename -s fex $1)bin
	# build fex
	sunxi-fexc -I fex -O bin $1 $binName 
else
	binName=$1
fi

cp $binName /boot/bin
cp -sf /boot/bin/$(basename $binName) /boot/script.bin
mkimage -C none -A arm -T script -d /boot/boot.cmd /boot/boot.scr

