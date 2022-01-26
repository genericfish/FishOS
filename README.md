# FishOS

Currently a UNIX-like kernel for x86_64 compatible processors.

## Build Instructions

### Prerequisites
- `qemu-system-x86_64` ~ 6.2.0
- `cmake` ~ 3.22.1
- `ninja` ~ 1.10.2
- `gcc` ~ 11.2
- `wget` ~ 1.21.2
- 

### Building
The toolchain can be built by running `Toolchain/Build.sh` from the top level project directory.

Then, make a new directory with `mkdir -p Build` then `cd Build`.

Inside the build directory, run `cmake -G Ninja ..`.

Finally, to run FishOS run `ninja run` inside the build directory.
