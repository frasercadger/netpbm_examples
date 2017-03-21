# NETPBM_EXAMPLES #

## Overview ##

This is intended as a collection of simple examples/tutorials using the libnetpbm library.

netpbm is a family of simple image formats; https://en.wikipedia.org/wiki/Netpbm_format.

libnetpbm is an open source C library for manipulating them.

See http://netpbm.sourceforge.net/doc/libnetpbm.html for more on libnetpbm

My motiviation is to learn more about image processing myself, and share this with others.

## Examples ##

Right now there are none!

My first intended example is a simple program to copy one pbm file to another, using
libnetpbm functions to read a pbm image header, create a new pbm file with the header
data, then copy the actual image. The idea is to show how the functions for opening,
reading, and writing pbm files can be used. It's a bit of a toy application, as you'd
normally do a straight copy with cp!
