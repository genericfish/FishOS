#!/usr/bin/env bash
set -e

# GCC/Binutils information
BINUTILS_VERSION=2.36.1
BINUTILS_NAME="binutils-$BINUTILS_VERSION"
GCC_VERSION=11.1.0
GCC_NAME="gcc-$GCC_VERSION"

# Variables
DIR=$(pwd)
NTHREADS=$(($(nproc --all) + 1))

export PREFIX="$DIR/Local"
export ARCH=i686
export TARGET=$ARCH-elf

if [[ ":$PATH:" != *":$PREFIX/bin:"* ]]; then
    export PATH="$PREFIX/bin:$PATH"
fi

rm -rf $DIR/Build
mkdir -p $PREFIX

# Flags
export CFLAGS="-g0 -O2"
export CXXFLAGS="-g0 -O2"
MAKEFLAGS="-j$NTHREADS"

# Binutils
cd $DIR

mkdir -p $DIR/Build/binutils
cd $DIR/Build/binutils

$DIR/Source/$BINUTILS_NAME/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror

make $MAKEFLAGS
make $MAKEFLAGS install

# GCC
cd $DIR

which -- $TARGET-as || echo $TARGET-as is not in the PATH

mkdir -p $DIR/Build/gcc
cd $DIR/Build/gcc

$DIR/Source/$GCC_NAME/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers

make $MAKEFLAGS all-gcc
make $MAKEFLAGS all-target-libgcc
make $MAKEFLAGS install-gcc
make $MAKEFLAGS install-target-libgcc
