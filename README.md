[![Release](https://img.shields.io/github/v/release/GrandOrgue/GoOdf)](https://github.com/GrandOrgue/GoOdf/releases)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

# GoOdf

GoOdf is a tool for creating and editing organ definition files for GrandOrgue. Opening large and complex .organ files can take a little while for the software to process, so please be patient if you try it.

The software is currently mainly tested on Linux, but building for Windows is possible and crossbuilds are released here on GitHub.

## Current status
GoOdf is hosted on Github. The repository can be found at [https://github.com/GrandOrgue/GoOdf](https://github.com/GrandOrgue/GoOdf).

A video/screencast series on how to use GoOdf can be found at https://www.youtube.com/playlist?list=PLNz9AJhjubTdw0pV2nZC_XfphJhJV5ly3

Creation of an usable odf for GrandOrgue with GoOdf fully works, though it's possible that some bugs still exist. It's still recommended to create the structural elements of the organ first - starting with setting the very base path where the .organ file is expected to reside.

Please be aware that currently, due to changes in the GrandOrgue software, from the version 0.11.0 of GoOdf the .organ files written will require a GrandOrgue version 3.14+ to work! If compatibility with older versions of GO is important one must also use an older version of GoOdf (like 0.10.0) to write the odf.

Note that the way to "save" the work done on an organ is by writing out the ODF (from file menu in the software or Ctrl+S), Save as is accomplished just by changing the organ definition file name. Since version 0.4 it's possible to also read/edit already existing .organ files. **However**, **be very mindful when using this feature!** It's possible that some bugs still exist that could potentially corrupt an ODF file. **Always work on copies or on files you don't mind having destroyed!** Also note that GoOdf always will convert older type of .organ files to conform to the new panel format - *which means that older .organ files will be modified*! To ensure correct reading of (numerical) values from the .organ files, any leading or trailing whitespace as well as comments on the same line as a key/value pair will be stripped, even in the read source file! **Again, work on copies** as it's better to be safe than sorry. **You have been warned!** All that said, if you encounter problems that are repeatable, please report them as issues at https://github.com/GrandOrgue/GoOdf/issues so that the software might be improved.

Rendering of the GUI panels is now possible, but it likely needs more work before it's fully reliable. Possibility to adjust positioning of smaller images and GUI elements directly on the graphical rendering is implemented since 0.7 either with mouse or with arrow keys. Changing order of switches is implemented with drag and drop in the tree, note that in GrandOrgue it's only possible for a switch to reference another switch with a lower number than itself. If you drag switches to other positions references can be invalidated and removed (only the reference is removed, not any switch). Re-ordering of many other elements in the [Organ] tree with drag and drop can also be done.

## Future plans
- Testing the ability to both write and read .organ files will need to continue so that they both works really well without any bugs.
- The graphical rendering of panels most likely still needs more work so that it should display the panels exactly like GrandOrgue will when scaled at 100%.
- Converting selection on existing GUI Panel display to a new panel.

## Building from sources
Normal development tools for c++ is needed as well as cmake, imagemagic and wxWidgets.

The build process on Linux is simple. In the GoOdf base directory, just create and cd into a separate build directory

```
mkdir build
cd build
```

and issue the command:

```
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

(or change to -DCMAKE_BUILD_TYPE=Release) and when that's finished enter:

```
make
```

and the program will compile and the binary will be available in the bin directory.
