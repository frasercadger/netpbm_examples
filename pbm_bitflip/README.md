# PBM_BITFLIP #

## Overview ##

This example accepts as input an image from the netpbm family, then flips each of its image bits such that if a bit is 0, it is now 1 
and if it was 1 it is now 1. This is intended as a simple means of showing how to manipulate a netpbm image.

## Note on libnetpbm representation of image data ##

libnetpbm uses the typedef tuple to represent the raster data of an image (i.e. the image itself distinct from the header). To quote 
the documentation; 'A PAM image describes a two dimensional grid of tuples. The tuples are arranged in rows and columns. The width of
the image is the number of columns. The height of the image is the number of rows. All rows are the same width and all columns are 
the same height'. 

In order to manipulate the image itself, we must read each tuple one at a time and then flip all of its bits 
before writing the modified tuple to the output image. The pbm_copy example introduced the pnm_readpamrow() and pnm_writepamrow() 
functions which will be used to read the original data  from the input image and then write the modified data back to the the output 
image as with pbm_copy. The difference being that inbetween the read and write we will modify the tuple row.

## Tuples and samples ##

The read and write functions above both take as input a pointer to a tuple value. 'tuple' is itself a typdef for for a pointer to 
sample, with sample itself being a typedef for unsigned long, (i.e. tuple is essentially a pointer to unsigned long). Both of these 
are defined in netpbm/include/pam.h. pam.h also tells us that tuple is 'an array such that tuple[i-1] is the  ith sample (element) 
in the tuple. It's dimension is the depth of the image'.

## Tuples and tuple rows ##

