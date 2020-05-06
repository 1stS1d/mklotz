# mklotz
A python3 rewrite of Marv Klotz's classic programs.  Designed to work in any python 3 environment, including micropython.

These are designed to be close to a 1-1 copy of Marv Klotz's programs from http://www.myvirtualnetwork.com/mklotz/


Currently converted programs:
- \MANUF\3WIRE
- \MANUF\ROTARY
- \MANUF\SINEBAR (EVERYTHING DONE EXCEPT FOR FINDING THE INDIVIDUAL GAGE BLOCKS)
- \MANUF\BOLTCIRC
- \MANUF\COMPOUND
- \MANUF\DALLOW
- \MANUF\KNURL
- \MANUF\DOT
- \MANUF\DPLATE
- \MANUF\DIAM
- \MANUF\GAGE
- \MANUF\FEED
- \MANUF\FITS
- \MANUF\ECCENT
- \MANUF\SPEED
- \MANUF\BEND

Programs to be converted soon:
- \MANUF\DRILL
- \SUBS\SPRING

To-do List:
1. Create two directiories for short file names (DOS compatability) and long file names (more descriptive)
2. Compile python programs into a single .exe file to mimick mklotz's original programs
3. Create a list to help you find/figure out what each program is for.
4. Figure out how releases work
5. Add option to print result to the console or a file for the programs that normally only print to a file

Change-Log:
v0.90:
- Finished BEND.PY

v0.80:
- Finished SPEED.PY
- Added .gitignore for VSCode's settings.json
- Changed changelog to reverse chronological order

v0.70:
- Finished ECCENT.PY
- Finished ECCENTUB.PY

v0.60:
- Finished FITS.PY

v0.50:
- Finished FEED.PY
- Updated to-do list

v0.49:
- Added \For upython\ directory for all of the currently converted programs
- Updated README completed list
- Fixed formatting on BOLTCIRC.PY
- Fixed formatting on DOT.PY
- Fixed formatting on KNURL.PY
- Tested pyinstaller & py2exe on BOLTCIRC.PY in the \Compiled Executables directory
- Fixed a promt on GAGE.PY

v0.45:
- Finished DIAM.PY

v0.41:
- Continued work on DIAM.PY
    - Switched to using regular expressions instead of mklotz's read algorithm for more versatility
- Changed DIAM.DAT to DIAMPY.DAT

v0.40:
- Started work on GAGE.PY
- Changed GAGE.PY to use 2 separate data files

v0.35:
- Finished GAGE.PY
- Added metric conversion for gages

v0.30:
- Started work on DIAM.PY
- Updated template.py

v0.22:
- Updated to-do list

v0.21:
- Fixed README.md formatting

v0.20:
- Finished DOT.PY
- Added template.py to root
- Updated README.md

v0.11:
- Fixed README.md formatting

v0.10:
- Added unzipped files from Marv Klotz's programs
- Added current python3 conversions

v0.00:
- Added readme & license

#
FAQ:
- Q: What's the upython.exe for?
    - A: That's micropython that's been compiled for FreeDOS (but works with any DOS from experience). Taken from https://github.com/pohmelie/micropython-freedos

- Q: I don't like the default values for some of the inputs.
    - A: Well change them! If you see a 'vin' command, the 2nd argument is the default value.

- Q: When will this be done?
    - A: Whenever I get around to it. This really is just for fun.  I (hopefully) plan to finish all of the \manuf programs, and possibly some of the submissions (if source code is available)

- Q: Too many decimal places!
    - A: In most cases I tried to round the answers to the tenths of and inch, as that is the usual practical limit of measuring normal things in a shop.  If you need more change it!  If this is a common request I might go back and add a "rounding" variable at the top of each file for people to change.

- Q: Why is are the data (.DAT) files different for the python programs?
    - A: Python handles files differently than C (obviously). In many cases I found it easier to change up the data structure so that the files could be handled with regular expressions, making it easier to modify the format, and allow as many entries as your RAM space can handle.