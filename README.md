# GOODF

GOODF is a tool for creating organ definition files for GrandOrgue. Editing of existing .organ files is implemented since version 0.4. Opening large and complex .organ files can take a little while for the software to process, so please be patient if you try it.

The software is currently mainly tested on Linux, but building for Windows is possible and crossbuilds are released here on GitHub.

## Current status
GOODF is hosted on Github. The repository can be found at [https://github.com/larspalo/GOODF](https://github.com/larspalo/GOODF).

Creation of an usable odf for GrandOrgue with GOODF fully works, though it's possible that some bugs still exist. It's still recommended to create the structural elements of the organ first - starting with setting the very base path where the .organ file is expected to reside.

Note that the way to "save" the work done on an organ is by writing out the ODF (from file menu in the software or Ctrl+S). Since version 0.4 it's possible to also read/edit already existing .organ files. **However**, **be very mindful when using this feature!** It's possible that some bugs still exist that could potentially corrupt an ODF file. **Always work on copies or on files you don't mind having destroyed!** Also note that GOODF always will convert older type of .organ files to conform to the new panel format - *which means that older .organ files will be modified*! To ensure correct reading of (numerical) values from the .organ files, any leading or trailing whitespace as well as comments on the same line as a key/value pair will be stripped, even in the read source file! **Again, work on copies** as it's better to be safe than sorry. **You have been warned!** All that said, if you encounter problems that are repeatable, please report them as issues at https://github.com/larspalo/GOODF/issues so that the software might be improved.

A very basic rendering of the GUI panels is now possible, but it still needs more work before it's fully reliable and usable. Presently only the transparency of .png files are likely to work, the masks of .bmp files needs more work to implement.

## Future plans
Testing the ability to both write and read .organ files will need to continue so that they both works really well without any bugs. The graphical rendering of panels also needs more work so that it should display the panels exactly like GrandOrgue will. The possibility to adjust GUI element positioning directly on the graphical rendering is also a possible future improvement.

## Building from sources
Normal development tools for c++ is needed as well as cmake, imagemagic and wxWidgets.

The build process on Linux is simple. In the GOODF base directory, just create and cd into a separate build directory

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
