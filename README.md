chromatic
=========

A small musical utility to break down chords, scales and progressions.
Currently used from the command line.

Usage
-----

For simplicity, chromatic only speaks in sharp notes, as opposed to flats.
Only triad chords are supported, and are notated in guitarists' shorthand as follows:
* C - C Major
* Cm - C Minor
* Ca - C Augmented
* Co - C Diminished
* Csus4 - C Suspended Fourth
* Csus2 - C Suspended Second

chromatic is case insensitive.

### Displaying a chord

    chromatic.exe chord <chord shorthand>

For example,

    D:\dev>chromatic chord G#sus2
    Chord G# Suspended Second:
    - G#-A#-D#

### Displaying a scale

    chromatic.exe scale <scale shorthand>

For example,

    D:\dev>chromatic scale F#m
    Scale F# Minor:
    - F#-G#-A-B-C#-D-E
    Chords in F# Minor:
    - F# Minor
    - G# Diminished
    - A Major
    - B Minor
    - C# Minor
    - D Major
    - E Major

### Displaying a chord progression

    chromatic.exe progression <progression> <scale shorthand>

For example,

    D:\dev>chromatic progression i-vi-iv-v F#m
    Chord progression i-VI-iv-v in F# Minor:
    - F# Minor
      F#-A-C#
    - D Major
      D-F#-A
    - B Minor
      B-D-F#
    - C# Minor
      C#-E-G#

Download
--------

Most recent Win32 build here: [chromatic/chromatic.exe](https://github.com/downloads/noorus/chromatic/chromatic.exe)

Building
--------

Requires [boost](http://www.boost.org/) in global includes.
Comes with a VS2008 solution, but should port trivially to other platforms.