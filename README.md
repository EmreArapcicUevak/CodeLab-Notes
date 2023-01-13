# CodeLab-Notes

CodeLab-Notes is a project for the subject [CS103](https://ecampus.ius.edu.ba/syllabus/cs103-introduction-programming).

## Project Overview 

CodeLab-Notes is a simple code editor that was made in around 13 \~ 15 days. It was worked on by [Emre Arapcic-Uevak](https://github.com/EmreArapcicUevak) & [Vedad Siljic](https://github.com/VedadSiljic) for their course graduation project. The application was made
using the [*QT Framework*](https://www.qt.io/).

## The Code Editor

<div align="center">
	<img src="./documentation/Images/CodeLabNotesPresentationShot.png" title="CodeLab-Notes" alt="CodeLab-Notes" width="87%"/>&nbsp;
</div>

CodeLab-Notes can do:
* Multiple File Opening / Saving / Tracking / Modifying / Updating / Removing
* Auto save per tab change
* Different Syntax Highlighting
* Free used memory even on a faulty programm exit (preventing memory leaks)

## How to run the application

### Running without QT Creator

Currenty we only exported the *windows* version to the GitHub, so if you are a MacOS or a Linux user feel free to export your the program on your own using our source code, or get QT Creator and run it on the CMake file. If you are a windows user
all that you have to do is go to the "build" folder and run the executable file provided in the files. You can click [here](https://github.com/EmreArapcicUevak/CodeLab-Notes/tree/readMe/build/windows/x64) to go to the before mentioned folder.

### Running with QT Creator

All you have to do is make sure to get the QT 6.4 or any version higher that is compatible, get CMake and MinGW 64x to be able to compile everything needed, once you are done getting everything ready you can build the [CMake file](https://github.com/EmreArapcicUevak/CodeLab-Notes/blob/readMe/CMakeLists.txt).
