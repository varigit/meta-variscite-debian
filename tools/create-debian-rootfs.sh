#!/bin/bash

# Set variables
ARCH=arm64
RELEASE=bookworm
TARGET_DIR=./debian-rootfs
MIRROR=http://deb.debian.org/debian
DATECODE=1
TARBALL=debian-${RELEASE}-rootfs-$(date +"%m-%d-%Y")-${DATECODE}.tar.zst

# Run debootstrap
sudo debootstrap --arch=$ARCH --variant=minbase --verbose --no-check-gpg $RELEASE $TARGET_DIR $MIRROR

# Create tarball using zstd
sudo tar -c --zstd -f $TARBALL -C $TARGET_DIR .

# Output
echo "Debian rootfs has been created and packed into $TARBALL"
