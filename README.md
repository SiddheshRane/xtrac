# XTRAC2.0
--------

## SYNOPSIS
 xtrac is a file compression program that converts files to the .xtr format.
 It is ideal for files with multiple repeating words or phrases.

## TARGET INPUT FILES FOR COMPRESSION
 Files must be text files encoded in ASCII.Though any text files might work, it works better on the following 
 - Human languages in any script delimited by space, comma, period.
 - Programming language source code.
 - Markup languages like XML, HTML and DSL like CSS.
 
## USAGE
 xtrac is available both as a non-GUI service as well as user-friendly terminal GUI.
 It can be called with parameters specifying the files to be compressed
 either from terminal or from another application.
 
## LIMITATIONS
 xtrac can currently work only on text files, particularly ASCII characters(8-bit). There is no support for UNICODE(16-bit).
 Though it can be run on binary files, compression is not guaranteed.
 The compression ratio depends largely on the contents of the file; particularly files with many repeating elements such as
 song lyrics, source code etc. have better compression.
 
## DEPENDENCY
 The GUI front end depends on ncurses. If the client system doesn't have ncurses pre-installed then linking will fail.
