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
in the tuple. It's dimension is the depth of the image'. So, a tuple can be considered an array of samples.

The width of the tuple is the number of samples, which can be found in the 'depth' field of the netpbm header. So for a depth of 1,
a tuple is an array of length 1 (i.e. tuple t[1]), for a depth of 8 tuple's length is 8 (i.e. tuple t[8]). With the number of samples
in a tuple known, the last piece of information we need is the size of each sample. Again, this can be found in the netpbm header,
under the bytes_per_sample field.

So, if we have an image with a depth of 8, and which uses 1 byte per sample we can manipulate a tuple as follows:

/* Pseudocode */
/* Assumes t has been allocated an array of samples*/
for(int i 0; i < 8; i++)
{
    uint8_t tmp = (uint8_t) t[1];
    /* Modify tmp*/
}

Note, the above uses a uint8_t to represent an individual sample (with an explicit cast from t[1]), if the value of bytes_per_sample 
is 1byte (i.e. 8bits) then this is perfectly safe (albeit not portable for instances where sample size is not 1byte).

A more portable method, would have tmp as unsigned long (the type of sample) and use bytes_per_sample to identify the relevant bits. 
I.e. for a value of 1byte, we only need to modify the last 8bits of tmp. Similarly, the hardcoded 8 in the for() loop could be
replaced with the value of depth.

## Note on the size of a sample ##

As mentioned above, the bytes_per_sample variable contains the number of RELEVANT bytes in a sample, and the type of each sample is 
unsigned long. If you're worried about the fact that unsigned long can vary depending on platform, and this could cause some issues
it's worth pointing out that pam.h states 'a sample is always less than 1 << 16', while the C Specification states that an unsigned
long must be capable of holding values of [0-4,294,967,295], therefore rest assured that regardless of the platform the maximum
possible size of a sample will always be the same.

## Tuples and tuple rows ##

So we now know how to manipulate a tuple (which is a pointer to an array of samples), but when we call pnm_readpamrow() we don't 
get a tuple. Instead we pass through a tuple pointer. If a tuple is a pointer to an array of samples, then a tuple pointer is a
pointer to a pointer of an array of samples. Can we therefore assume, that what we get back from pnm_readpamrow is an array of 
tuples?

To quote the documentation, pnm_readpamrow() 'returns the row as the array of tuples tuplerow, which must already have its column 
pointers set up so that it forms a C 2-dimensional array'.

Therefore, the output of 'pnm_readpamrow(&tuplerow)' is an array of tuples which can be addressed as tuplerow[i][j] where i is an
index representing each individual tuple in a row, and j indexes the samples within a tuple.
