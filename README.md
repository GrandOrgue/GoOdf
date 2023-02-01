# GOODF

GOODF is a tool for creating organ definition files for GrandOrgue. Editing of existing .organ files will be implemented in the future.

It's currently only tested on Linux, but building for Windows should be possible.

## Current status
GOODF is hosted on Github. The repository can be found at [https://github.com/larspalo/GOODF](https://github.com/larspalo/GOODF).

Lots of bugs and issues needs to be fixed before this software will be fully functioning. In the current state it's highly recommended to make all the structural work on the organ first and only when that's finished should any gui rendering on panels be done.

Also note that it's currently not possible to "save" any work except by writing out the odf file from the software. Nor is it at the moment possible to read/edit already existing .organ files.

## Future plans
The first step will be to fix the obvious bugs and issues that affect normal usage. Next will come the ability to read existing .organ files which also makes it possible to continue working on the odf.

## Building from sources
Normal development tools for c++ is needed as well as cmake, imagemagic and wxWidgets.

The build process on Linux is simple. In the GOODF base directory, just create and cd into a separate build directory

mkdir build
cd build

and issue the command

cmake .. -DCMAKE_BUILD_TYPE=Debug

and when that's finished enter

make

and the program will compile and the binary will be available in the bin directory.
