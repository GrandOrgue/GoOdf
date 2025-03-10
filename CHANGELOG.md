# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Option to create new panel from selection on existing panel display. (TODO)

## [0.15.1] - 2025-03-10

### Fixed

- Reading of FourCC values in WAV/WavPack files so that sample file details are correctly shown on all platforms.
- Reading of all blocks and sub-blocks in a WavPack file and a few related bugs.
- Unintentional entries of comments in textcontrols by prohibiting usage of semicolon (;).
- Removed restriction that prevented using spaces in the ODF file name.
- Trailing whitespaces in textctrls that should be removed when writing an ODF.

## [0.15.0] - 2024-07-27

### Added

- Option to duplicate an existing panel by right-clicking on it in the organ tree.
- Setter element CrescendoOverride.

### Fixed

- Display of correct default text on setter button elements.
- Crash caused by trying to open recent file that doesn't exist anymore.
- Bug that prevented changing label position by dragging or keys if FreeXPlacement and FreeYPlacement was false.
- Bug that caused a read label placement at the last column to be invalid.
- Crash happening on windows port if organ tree had wrong focus when opening an .organ file.
- Copy pipe offset feature to also set HasIndependentRelease value from source pipe to target(s).
- Issue/Bug on the msw port in the PipeDialog that would cause forcing selection of IsPercussive=Y when navigating with "Next" button to the last pipe
- Direct right-click on a pipe in rank pipe tree that previously didn't create a true selection of a pipe in the msw port.
- Restore focus to main window after opening an .organ file on the msw port (make sure it's raised).
- Not writing Key999Width for sharps when not necessary (could potentially mess up the layout of keys).
- Take tremulants into accound when checking if a switch is referenced anywhere.

## [0.14.0] - 2024-07-02

### Added

- Option to load pipe samples with Pipe999IsTremulant value set to be played, if the associated wave-based tremulant is off.
- Log warning if drawstop has other function than Input and doesn't have any referenced switches.
- Log warning if no windchestgroups exist in organ when opening/saving/writing.
- Log warning if no GUI Element exist on main panel when opening/saving.
- Log warning if no windchestgroup is set for rank or internal rank of a stop when opening/saving.
- Log warning if a switch has function Input but is not referenced anywhere.

### Changed

- The tooltip help text for some tremulant attributes and all drawstop Function choice.
- State of drawstop type attributes of Default to engaged and GCState options to reflect Function choice.

## [0.13.0] - 2024-06-19

### Added

- Builds for macOS, both Intel and arm64 architectures.

### Fixed

- Bug that prevented removal of last switch from list of available switches for stops, couplers, tremulants and divisional couplers.
- Bug that prevented removal of last rank from list of available ranks for stops.
- Bug that prevented removal of last manual from list of available manuals for divisional couplers.
- Updating windchest wxChoice size for rank (and stop internal rank) to expand as needed.

## [0.12.2] - 2024-05-22

### Changed

- Tools->Parse Legacy X-fades option naming to Import Legacy X-fades and to act directly on the currently open organ file.
- Size of right side panels to have an uniform minimum size.
- All row distances in displaymetrics panel are equal.

### Fixed

- Setter element labels retain their naming identification from type in the tree.
- Clicking on a GUI Element on the panel representation view should only select the uppermost element and not all underlying elements too.
- Display first key of a manual as a whole even if not an E or B provided second key is displayed as a natural.
- Crash and add log entries if divisional referenced a tremulant or switch number higher than available on the manual in question.

## [0.12.1] - 2024-05-08

### Changed

- Gui Label elements update their display name in tree to be easier to identify.
- Changing function to Input when it was something else now also removes any existing Switch references.
- Changing function to Not when having more than one Switch reference removes all but the first reference.
- Divisional coupler switch listboxes now allow multiple selection like other drawstop types.
- Updated help files to also reflect app name change.

### Fixed

- Warn about and ignore duplicate Switch entries when reading an .organ file.

## [0.12.0] - 2024-05-05

### Added

- Option to parse/convert/copy legacy loop/release X-fades that are supposed to be inherited by other pipe attacks/releases (like they did in a version pre GO 3.14.0) when opening an .organ file.
- Log window to display some messages/warnings/hints etc.

## [0.11.1] - 2024-04-22

### Changed

- CMake build system now take changes to version.txt file into account.

### Fixed

- Crash when writing .organ file and a rank tried to access non existant windchest.
- Loading of an .organ file with only a pedal as manual.

## [0.11.0] - 2024-03-30

### Added

- Option to import stop/rank definitions from other .organ files.
- HasIndependentRelease option for percussive ranks and pipes.
- Percussive and HasIndependentRelease is also possible to set at [Organ] and [WindchestGroup999] levels.
- AmplitudeLevel, Gain, PitchTuning, PitchCorrection and TrackerDelay is now available for [WindchestGroup999].

### Changed

- Build system on GitHub now allow testing builds in between releases from Actions.
- AppImage build is now available.
- Show what element actually is represented by each Gui Element.
- Percussive and HasIndependentRelease values (default false) are inherited from [Organ]->[WindchestGroup]->[Rank]->[Pipe]. Note that this change in behaviour will cause odfs produced/saved with this version of GoOdf to be incompatible with earlier versions of GrandOrgue than 3.14+.
- Increased maximum allowed panel size to 32000 pixels.

### Fixed

- Click/drag adjustment within selection rectangle should not select/change underlying elements.
- Text rendering on enclosure GUI element.
- Reading gui drawstop button element image number 7.

## [0.10.0] - 2024-02-22

### Added

- Possibility to have multiple GUI Elements selected/moved on the panel display.
- Possibility to create selection rectangle and a right click popup-menu for options.

### Changed

- App name to camel case and transferred project to GrandOrgue organization.
- Windows cross-build is now 64 bit.

## [0.9.2] - 2024-02-05

### Added

- Option to create/add multiple tremulants at once.
- New dialog showing audio sample file details.
- Button to auto-detect harmonic number for rank (mostly useful for Mixtures and similar stops).

### Changed

- Allow up to 999 tremulants.
- Loop and release crossfade values are now available on sample level.

### Fixed

- Percussive pipe with release writing bug.
- Writing DisplayAsPiston value for divisional coupler.

## [0.9.1] - 2024-01-02 

### Added

- Tooltip display option is now stored in config file.
- Main window size and position as well as the sash position is saved in config file.
- Keyboard navigation, with left/right arrow keys, in pipe dialog
- SpinCtrl for Enclosure to display and set MIDIInputNumber.
- A synchronizing mechanism for rank and stop internal rank reading options, which also are stored in config file.
- Option to also copy/replace loops in other attacks coming from the same directory.
- Global default paths for organ and cmb directories, choices stored in config file.

### Fixed

- Correct choice of built-in keyboard bitmap for first key when other than 036-C.
- A read/found incorrect usage of DispLabelText is converted to the right TextBreakWidth instead.
- Wrong file modified notification for some changes.
- Crash caused by invalid image.

## [0.9.0] - 2023-11-26

### Added

- Option to import voicing data from a .cmb file.
- Tooltips that can be enabled/disabled from menu.
- Recently used files history.
- Config file to store user choices.

### Changed

- Display of decimal points of SpinCtrlDoubles to match GO.

### Fixed

- Bug when changing the number of keys of a manual.
- Bug when last displayed key of a manual was B.
- Sizer flag warnings when building Debug version with wxWidgets 3.2
- Manual layout to better match GO.

## [0.8.0] - 2023-11-03

### Added

- Option to copy pipes offset with automatic PitchTuning calculated.
- Flexible pipe reading option dialog.

### Changed

- Manually added pipe will now replace a DUMMY pipe.
- Expand currently selected pipe when finished with editing properties.
- Remember which notebook page was last used for a stop and continue using that page if possible.

### Fixed

- Updating pipe root path when reading an .organ file.
- Crash when deleting a Switch that wasn't referenced in a manual but was in a child element (divisional, coupler or stop).
- Writing out harmonic number 8 if it was necessary.
- Writing, or not writing, some odf lines for couplers depending on its type.

## [0.7.3] - 2023-10-10

### Added

- Drag and drop re-ordering of panels.
- Showing organ name in frame title.

### Fixed

- Crash when user tried to drag some elements.
- False modification notice from wxTextCtrls.

## [0.7.2] - 2023-08-23

### Added

- Handling of panel representations larger than the actual screen size.
- Using image mask for transparency in GUI representation when it exist for the element.
- Direct usage of wxFontDialog for buttons and labels.

### Changed

- Improved close operation if un-saved modifications exist.
- Reduced the amount of flickering when a GUI element is moved by held down keyboard key (arrows).
- Increased loop and release cross fade length to match increased values in recent GrandOrgue versions.
- Made font creation more consistent for buttons and labels when reading.
- Increment as whole numbers for wxSpinCtrlDouble while still keeping six digits decimal precision.

### Fixed

- Better checking if organ is modified.
- Better handling when opening/reading an odf including when cancelling the open operation.
- Bug that prevented writing/reading a manual as pedal.
- Enforce writing (default) offset if it was originally present in the read odf.
- A few font related bugs.

## [0.7.1] - 2023-08-12

### Added

- Possibility to move/re-order Gui Elements within the same panel.
- Possibility to move stops by drag and drop both within manuals and to other manuals.
- Update x/y position values on any GUI element panel when elements are moved on the representation panel.
- Drag and drop sorting of ranks in tree.
- Drag and drop sorting of windchestgroups in tree.
- Keyboard input (arrow keys) for fine tuning element x/y position on panel representation.
- Drag and drop sorting of manuals in tree.

### Changed

- Image/mask on and off handling to not depend on wrong things and behave correctly when any of them are removed.
- Allow multiple selections in listboxes for adding and removing switch references.

### Fixed

- Made sure to update size of representation panel containing window when it's shown and avoid double drawing.

## [0.7.0] - 2023-07-27

### Added

- Basic possibility to drag (most) elements to position on GuiRepresentation panel.
- Option to add enclosure bitmaps and images in one step.
- Possibility to sort/re-order switches by drag and drop in tree.

### Fixed

- MouseRadius reading default value for buttons.

## [0.6.3] - 2023-07-25

### Changed

- Allow all GUI elements with matching base type as targets for copying operations.
- Speed up reading .organ file by not constantly updating keyinfo while adding keytypes.

### Fixed

- Bug that caused crash if an open operation was cancelled.
- Using correct initial bitmap size for divisionals and generals when displayed as piston by default.
- Bug that prevented coupler state being saved to a (odf) general.
- Bug that prevented changing some attributes of buttons using builtin bitmaps.
- Crash by not trying to create bitmap from not (yet) ok image.
- Crash caused by possible double removal of references to windchest.

## [0.6.2] - 2023-07-03

### Fixed

- Font scaling in the same way as GrandOrgue does.
- Draw GUI elements respecting smaller width value than the bitmap width.

## [0.6.1] - 2023-06-29

### Changed

- Pedal key layout algorithm.
- Updated GUI panel representation with images adjustments and also the panel has pedals option.
- Only force gui update if panel is shown.

### Fixed

- Drawing on the panel representation dialog for wxMSW.
- Reading builtin bitmap size as piston/drawstop to reflect previously read value.
- Updating the panel representation dialog title and redraw the content when another panel is selected.

## [0.6.0] - 2023-06-25

### Added

- GUILabel graphical representation.
- Graphical display of manuals.
- Some backgrounds.
- Enclosures to the panel representation display.
- Drawstop/piston positioning and text writing.
- Graphical panel rendering.

### Changed

- Update the panel representation when element properties changes.
- Text break as in GO.

### Fixed

- Tiling of images.
- Text placement.
- Displayed default text for setter divisionals and generals.
- Writing images with correct path separator.
- Reading CouplerType.
- Bug that made editing of builtin enclosure bitmap attribute values impossible.

## [0.5.2] - 2023-05-25

### Added

- help text to HarmonicNumber spinctrl explaining the value as pitch foot length.

### Changed

- Updated/added images from GrandOrgue.
- Remember borrowing from which manual and stop while working on same rank.
- Implemented possibility to create multiple winchestgroups, enclosures, ranks, switches and stops at once.
- Increased max number of enclosures to 999.

### Fixed

- Default display value of GeneralPrev and GeneralNext.
- Switch count for the function NOT.
- Corrected pitch tuning range for rank and pipe.

## [0.5.1] - 2023-05-09

### Added

- A faster procedure to remove/delete elements with same parent.
- Removed comments when reading .organ file.

### Changed

- Enforce writing a manual key width value to odf (if needed).
- Expanded organ and setter element listboxes.
- Disallowed adding name text for setter element labels.
- Stop exact MIDI number match checking if -, _ or . is found to allow for multiple samples added.
- Allowed more matches for copying Gui elements.
- Allow negative YOffset adjustments for manual keys.
- Reading of divisionals after the couplers for the manual are parsed.
- Updated help files.

### Fixed

- Wrong number of pipe removal
- Writing out the absolute position x and y values for a Gui Divisional.
- Image min and max values.
- Reading vertical screen size values.

## [0.5.0] - 2023-04-29

### Added

- Possibility to copy bitmaps and related settings from a Gui Element to other elements of similar type.
- Possibility to copy GUI manual key attributes from one added key(type) to others within same manual.
- Shortcuts to rank pipe tree as shown in menu but handled as key down events for selected item.

### Changed

- Keyboard shortcuts for the file menu.
- Ask for overwrite confirmation only first time, and confirm write operation first time only.
- Provided a tremulant with empty name a default value.
- Don't assume that value will be zero padded for stops, divisionals and couplers.

### Removed

- Compile options specific for the x86 family of computers.

### Fixed

- Update Gui Button max mouse and text width/height values when parameters have changed.
- Reading of loops specified in odf.
- Key type numbers possible override take into account the base key type.
- Update selected manual display key from the selected added key (number type).

## [0.4.3] - 2023-04-11

### Added

- Implemented keyboard shortcut Ctrl+S to write odf.
- Possibility to change font name and/or size of many elements from display metrics defaults.
- Possibility to revert back to using display metrics default name and/or size for individual elements easily.
- Possibility to empty existing organ info file value.

### Changed

- Default to an empty string as label name.
- Made GUI button and label panel remember last used image path.
- Updated help files.
- Set default font values from corresponding display metrics values.
- Parsed the Panel000 base part earlier when reading an existing .organ file.
- Disabled not used parameters for certain keytypes.
- Reading and writing of GUI enclosures to conform to GrandOrgue code requirements.

### Removed

- Write confirmation message.
- Comments occuring on the same line as a key/value pair.
- Starting, trailing whitespace in read config file values.

### Fixed

- A few issues mostly related to the reading of elements.
- MouseRectHeight calculation for button types when reading.
- Applying the read size to the font too.
- Writing of font size for enclosures.
- Font size bug for enclosures.
- GUI label attribute values to conform to GO specs.
- GUI manual values to conform to GO expectations.
- Referenced rank pipe count possible max value when checking read value.
- Gui button default/valid values both when reading and writing.
- Rank pipe count calculation while reading odf.
- Allowed manual index range while reading odf.
- Reference index when writing a reversible piston.
- True relative paths to the organ root.

## [0.4.2] - 2023-03-23

### Changed

- Provide default names for windchests.

### Removed

- Leading dot and path separator from paths.

### Fixed

- Bugs related to reading manuals.
- Correct button row/col reading ranges.

## [0.4.1] - 2023-03-22

### Changed

- Updated readme to reflect recent changes.

### Fixed

- Reading of GUI Elements for additional panels too.

## [0.4.0] - 2023-03-21

### Added

- Creation of release with tag from workflow builds.
- Complete reading of existing .organ files.

### Changed

- Included authors file in built packages.
- Support more different image suffixes in gui panels.
- MIDI note detection when reading pipes to also allow no leading zero.
- Implemented unison off option in coupler panel.

### Fixed

- Displaying of copyright.
- Tremulant release folder detection.
- Key press time extraction

## [0.3.0] - 2023-03-02

### Added

- A help system.
- Possibility to add only releases to pipes.
- Button to expand whole pipe tree.
- MIDIPitchFraction key for pipes to override sample value (or provide it).
- Colour choice and picker to Gui buttons and labels.

### Changed

- Allow multiple selections in the organ elements listbox for GUI Element creation.
- Stored full path to info file and fixed writing.

### Removed

- GCState writing when function is not Input.

### Fixed

- Enabled the attack properties copy button on changed options.
- Updating accessible pipes in stop from first referenced rank.
- Display in inverted state for other objects derived from button.
- Updating bitmap sizes for piston/drawstop.

## [0.2.0] - 2023-02-18

### Added

- Possibility to add more samples and specific tremulant samples.
- Implementation of tracker delay.
- Organ level pitch correction.
- Old style pipe referencing/borrowing.
- Possibility to reference following pipes too at once.

### Changed

- Update relative paths if odf path changes.

### Fixed

- Updating display name of GUI elements.
- Correct new organ creation.
- Removal of references if item itself is removed.

## [0.1.0] - 2023-02-02

### Added

- Very first build was released!

