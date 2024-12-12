#!/bin/bash

# Description:
# This script creates a minimal Debian root filesystem tarball using debootstrap.
# It has been tested in a Multipass Ubuntu 22.04 environment with the following steps:
# 1. Launch a Multipass instance:
#    $ multipass launch 22.04 --name ubuntu22-debootstrap --cpus 16 --memory 16G --disk 200G
# 2. Access the instance:
#    $ multipass shell ubuntu22-debootstrap
# 3. Install required dependencies:
#    $ sudo apt update && sudo apt install qemu-user-static binfmt-support debootstrap zstd tar -y
# 4. Run this script inside the Multipass instance. The script completes successfully with these dependencies.

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
