# GOODF

GOODF is a tool for creating organ definition files for GrandOrgue. Editing of existing .organ files will be implemented in the future.

It's currently mainly tested on Linux, but building for Windows is possible.

## Current status
GOODF is hosted on Github. The repository can be found at [https://github.com/larspalo/GOODF](https://github.com/larspalo/GOODF).

Creation of a usable odf for GrandOrgue with the software fully works, though it's possible that some bugs still exist. It's still recommended to create the structural elements of the organ first - starting with setting the very base path where the .organ file is expected to reside.

Note that it's currently not possible to "save" any work except by writing out the odf file from the software, nor is it at the moment possible to read/edit already existing .organ files.

## Future plans
If no obvious bugs or issues that affect normal usage is detected, next step will be implementing the ability to read existing .organ files - which also makes it possible to continue working on an odf. Graphical rendering of panels with the possibility to adjust GUI element positioning is also a future improvement.

## Building from sources
Normal development tools for c++ is needed as well as cmake, imagemagic and wxWidgets.

The build process on Linux is simple. In the GOODF base directory, just create and cd into a separate build directory

```mkdir build
cd build
```

and issue the command:

```cmake .. -DCMAKE_BUILD_TYPE=Debug
```

and when that's finished enter:

```make
```

and the program will compile and the binary will be available in the bin directory.
