# GOODF

GOODF is a tool for creating organ definition files for GrandOrgue. Editing of existing .organ files is implemented since version 0.4.

It's currently mainly tested on Linux, but building for Windows is possible.

## Current status
GOODF is hosted on Github. The repository can be found at [https://github.com/larspalo/GOODF](https://github.com/larspalo/GOODF).

Creation of a usable odf for GrandOrgue with the software fully works, though it's possible that some bugs still exist. It's still recommended to create the structural elements of the organ first - starting with setting the very base path where the .organ file is expected to reside.

Note that the only way to "save" any work done on an organ is by writing out the ODF (from file menu in the software). Since version 0.4 it's possible to also read/edit already existing .organ files. **However**, since this is a very new feature **be very careful with this!** It's likely that some bugs exist that could potentially destroy an ODF file. **Always work on copies or on files you don't mind having destroyed!** Also note that GOODF always will convert older type of .organ files to conform to the new panel format - *which means that older .organ files will be modified*! **Again, work on copies** as it's better to be safe than sorry. **You have been warned!** All that said, if you encounter problems that are repeatable, please report them as issues at https://github.com/larspalo/GOODF/issues so that the software might be improved.

## Future plans
Next step will be testing the ability to read existing .organ files thoroughly so that it works really well. Another plan for the close future is enabling the action (workflow) feature of GitHub to provide release builds of the software directly here. Graphical rendering of panels with the possibility to adjust GUI element positioning is also a possible future improvement.

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
