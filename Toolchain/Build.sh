#!/usr/bin/env bash
set -e

# Variables
FSH_TOPLEVEL=$(git rev-parse --show-toplevel)
DIR="${FSH_TOPLEVEL}/Toolchain"
NTHREADS=$(($(nproc --all) + 1))

# GCC/Binutils Information
BINUTILS_VERSION=2.37
GCC_VERSION=11.2.0

BINUTILS_NAME="binutils-$BINUTILS_VERSION"
GCC_NAME="gcc-$GCC_VERSION"

BINUTILS_TAR=${BINUTILS_NAME}.tar.xz
GCC_TAR=${GCC_NAME}.tar.xz

# GCC/Binutils Source URLs
BINUTILS_URL="https://ftp.gnu.org/gnu/binutils"
GCC_URL="https://ftp.gnu.org/gnu/gcc/"

BINUTILS_SOURCE_URL="${BINUTILS_URL}/${BINUTILS_TAR}"
GCC_SOURCE_URL="${GCC_URL}/${GCC_NAME}/${GCC_TAR}"

BINUTILS_SIG_URL="${BINUTILS_SOURCE_URL}.sig"
GCC_SIG_URL="${GCC_SOURCE_URL}.sig"

# Download GCC/Binutils Source
mkdir -p ${DIR}/Tarballs

if [ ! -f ${DIR}/Tarballs/${BINUTILS_TAR} ]; then
    wget ${BINUTILS_SOURCE_URL} -P ${DIR}/Tarballs/
fi

if [ ! -f ${DIR}/Tarballs/${GCC_TAR} ]; then
    wget ${GCC_SOURCE_URL} -P ${DIR}/Tarballs/
fi

# Download GCC/Binutils Signatures
mkdir -p ${DIR}/Signatures

rm -f ${DIR}/Signatures/gnu-keyring.gpg
wget https://ftp.gnu.org/gnu/gnu-keyring.gpg -P ${DIR}/Signatures

if [ ! -f ${DIR}/Signatures/${BINUTILS_TAR}.sig ]; then
    wget ${BINUTILS_SIG_URL} -P ${DIR}/Signatures/
fi

if [ ! -f ${DIR}/Signatures/${GCC_TAR}.sig ]; then
    wget ${GCC_SIG_URL} -P ${DIR}/Signatures/
fi

# Verify Signatures
gpg --verify --keyring ${DIR}/Signatures/gnu-keyring.gpg ${DIR}/Signatures/${BINUTILS_TAR}.sig ${DIR}/Tarballs/${BINUTILS_TAR}
gpg --verify --keyring ${DIR}/Signatures/gnu-keyring.gpg ${DIR}/Signatures/${GCC_TAR}.sig ${DIR}/Tarballs/${GCC_TAR}

# Cleanup existing files
rm -rf ${DIR}/Build
rm -rf ${DIR}/Local
rm -rf ${DIR}/Source

# Recreate directories
mkdir -p ${DIR}/Source
mkdir -p ${DIR}/Build
mkdir -p ${DIR}/Local

# Extract
tar -xf ${DIR}/Tarballs/${BINUTILS_NAME}.tar.xz -C ${DIR}/Source
tar -xf ${DIR}/Tarballs/${GCC_NAME}.tar.xz -C ${DIR}/Source

export PREFIX="$DIR/Local"
export ARCH=x86_64
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
